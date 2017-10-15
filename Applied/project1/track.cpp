#include "track.hpp"

// TODO

Track::Track(intmax_t ticks_per_beat)
{
	iterator = begin();
	ticks_per_beat_ = ticks_per_beat;
	testBool = false;
}

Track::~Track()
{
	eventList.clear();
}

intmax_t Track::getTicksPerBeat() const
{
	return ticks_per_beat_;
}


void Track::addEndEvent(intmax_t tick_time)
{
	EndTrackEvent e;
	MIDIEvent endEvent(e, tick_time);
	addEvent(endEvent);
}

void Track::addTempoEvent(intmax_t tempo, intmax_t tick_time)
{
	TempoEvent e(tempo);
	MIDIEvent tempoEvent(e, tick_time);
	addEvent(tempoEvent);
}

/** Add a NoteEvent to the track sequence.
* \param code the event code (8 = Off, 9 = On unless data2 = 0)
* \param data1 the first data byte (holding the note number)
* \param data2 the second data byte (holding the velocity)
* \param tick_time, the MIDI tick time of the event.
*/
void Track::addNoteEvent(uint8_t code, uint8_t data1, uint8_t data2, intmax_t tick_time)
{
	if (code != 8 && code != 9) { return; }
	bool on = true;
	if (data2 == 0 || code == 8) {
		on = false;
	}
	NoteEvent e(on, data1, data2);
	MIDIEvent noteEvent(e, tick_time);
	addEvent(noteEvent);
}
//events are ordered by tick time
//not sure if they come ordered in input so this is here
void Track::addEvent(MIDIEvent e)
{
	testBool = true;
	if (eventList.empty()) {
		iterator = eventList.insert_after(eventList.before_begin(), e);
	}
	else {
		eventList.insert_after(iterator, e);
		iterator++;
	}
	/*
	if (eventList.empty()) {
		iterator = eventList.insert_after(eventList.before_begin(), e);
		return;
	}
	//check beginning of list
	iterator = begin();
	if (e.getTickTime() <= iterator->getTickTime()) {
		iterator = eventList.insert_after(eventList.before_begin(), e);
		return;
	}
	//place event in correct location
	do {
		if (e.getTickTime() >= iterator->getTickTime()) {
			iterator = eventList.insert_after(iterator, e);
			return;
		}
		iterator++;
	} while (iterator != end()); */
}

void Track::popEvent() {
	eventList.pop_front();
}
bool Track::emptyList() {
	return eventList.empty();
}

bool Track::getBool()
{
	return testBool;
}
