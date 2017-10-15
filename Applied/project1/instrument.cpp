#include "instrument.hpp"


DefaultInstrument::DefaultInstrument(const Track & track)
{
	track_ = track;
	halted_ = false;
	numSamples = 0;
	currentTempo = 500000;
}

DefaultInstrument::~DefaultInstrument()
{
	currentEvents.clear();
}

bool DefaultInstrument::halted()
{
	return halted_;
}

//actually just passing in sample rate for deltaT realtime calculated on first line
double DefaultInstrument::sample(double deltaT)
{
	double currentTime = numSamples / deltaT;
	double noteSample = 0;
	auto it = track_.begin();
	bool passedCurrTime = false;
	//process event from track 
	while (track_.emptyList() || passedCurrTime) {
		double timeOn = it->getTickTime();

		//realtime calc
		timeOn = (it->getTickTime()*currentTempo) / (pow(10, 6)* track_.getTicksPerBeat());
		//attempts to process current events
		if (timeOn > currentTime) {
			passedCurrTime = true;
			break;
		}

		if (it->isNoteEvent()) {
			noteStruct note_;
			NoteEvent e = it->asNoteEvent();

			if (e.getNoteOn()) {
				note_.velocity = e.getNoteVelocity();
				note_.duration = .25; //default note length
				note_.timeTurnedOn = timeOn;
				currentEvents[e.getNoteNumber()] = note_;
			}//if note off remove note from current note map
			else {
				currentEvents.erase(e.getNoteNumber());
			}
			it++;
			track_.popEvent();
		}
		if (it->isTempoEvent()) {
			TempoEvent e = it->asTempoEvent();
			currentTempo = e.getTempo();
			it++;
			track_.popEvent();
		}
		if (it->isEndTrackEvent()) {
			endTrackTime = timeOn; //maybe dont need this
			//do halted here
			halted_ = true;
			it++;
			track_.popEvent();
		}
	}
	if (!halted_) {
		//iterate through notes when getting to a noteevent play for .25s 
		//check current notes
		double currNote;
		for (std::map < int8_t, noteStruct>::iterator it_ = currentEvents.begin(); it_ != currentEvents.end(); ++it_) {
			currNote = 200 * it_->second.velocity * envelope(currentTime - it_->second.timeTurnedOn) * sin(2 * PI_ * convertFreq(it_->first) * currentTime);
			noteSample += currNote;
		}
	}

	if (halted_) {
		noteSample = 0;
	}
	numSamples++;
	return noteSample;
	
}


double DefaultInstrument::envelope(double t)
{
	double val = 0.0;
	if (0 <= t && t <= 0.25) {
		val = t / .25;
	}
	else if (.25 < t && t <= .45) {
		val = 1 - (t - .25);
	}
	else if (.45 < t && t <= .8) {
		val = .8;
	}
	else if (.8 < t && t <= 1) {
		val = .8 - 4 * (t - .8);
	}
	else {
		val = 0.0;
	}
	return val;
}

double DefaultInstrument::convertFreq(int8_t index)
{//
	int note = index % 12;
	double frequency = 0.0;
	switch (note) {
	case 0:
		frequency = 26.163;
		break;
	case 1:
		frequency = 27.718;
		break;
	case 2:
		frequency = 29.366;
		break;
	case 3:
		frequency = 31.113;
		break;
	case 4:
		frequency = 32.963;
		break;
	case 5:
		frequency = 34.963;
		break;
	case 6:
		frequency = 36.9994;
		break;
	case 7:
		frequency = 39.20;
		break;
	case 8:
		frequency = 41.5305;
		break;
	case 9:
		frequency = 44.00;
		break;
	case 10:
		frequency = 46.6164;
		break;
	case 11:
		frequency = 49.388;
		break;
	default:
		frequency = 0;
		break;
	}
	int x = (index - note) / 12;
	//frequency = base note * 2^number of octaves above base index (index - mod12)/12
	frequency = frequency * pow(2, x);
	return frequency;
}
