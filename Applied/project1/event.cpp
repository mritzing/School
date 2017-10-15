#include "event.hpp"


TempoEvent::TempoEvent(intmax_t t)
{
	tempo = t;
}

TempoEvent::~TempoEvent()
{
}

intmax_t TempoEvent::getTempo() const
{
	return tempo;
}

NoteEvent::NoteEvent(bool on, int8_t notenum, int8_t notevelocity)
{
	on_ = on;
	notenum_ = notenum;
	notevelocity_ = notevelocity;
}

NoteEvent::~NoteEvent()
{
}

bool NoteEvent::getNoteOn()
{
	return on_;
}

int8_t NoteEvent::getNoteNumber() const
{
	return notenum_;
}

int8_t NoteEvent::getNoteVelocity() const
{
	return notevelocity_;
}

MIDIEvent::MIDIEvent(TempoEvent e, intmax_t ticktime)
{
	eventType = tempoEventType;
	eventUnion.tempoEvent_ = e;
	ticktime_ = ticktime;
}

MIDIEvent::MIDIEvent(EndTrackEvent e, intmax_t ticktime)
{
	eventType = endTrackEventType;
	eventUnion.endTrackEvent_ = e;
	ticktime_ = ticktime;
}

MIDIEvent::MIDIEvent(NoteEvent e, intmax_t ticktime)
{
	eventType = noteEventType;
	eventUnion.noteEvent_ = e;
	ticktime_ = ticktime;
}

MIDIEvent::~MIDIEvent()
{
}

bool MIDIEvent::isTempoEvent() const
{
	return (eventType == tempoEventType);
}

bool MIDIEvent::isEndTrackEvent() const
{
	return (eventType == endTrackEventType);
}

bool MIDIEvent::isNoteEvent() const
{
	return (eventType == noteEventType);
}

intmax_t MIDIEvent::getTickTime() const
{
	return ticktime_;
}

TempoEvent MIDIEvent::asTempoEvent() const
{
	return eventUnion.tempoEvent_;

}
NoteEvent MIDIEvent::asNoteEvent() const
{
	return eventUnion.noteEvent_;
}
