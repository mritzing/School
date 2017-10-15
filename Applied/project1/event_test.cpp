/** \file Unit tests for Event Module
*/
#include "catch.hpp"

#include "event.hpp"


TEST_CASE("TempoEvent", "[event]") {
	TempoEvent tempTempo(500000);
	REQUIRE(tempTempo.getTempo() == 500000);
}

TEST_CASE("NoteEvent", "[event]") {
	NoteEvent tempNote(true, 8, 15);
	REQUIRE(tempNote.getNoteOn());
	REQUIRE(tempNote.getNoteNumber() == 8);
	REQUIRE(tempNote.getNoteVelocity() == 15);
}

TEST_CASE("MIDINoteEvent", "[event]") {
	NoteEvent e(false, 10, 1);
	MIDIEvent tempMIDI(e, 100);
	REQUIRE(tempMIDI.isNoteEvent());
	REQUIRE_FALSE(tempMIDI.isTempoEvent());
	REQUIRE_FALSE(tempMIDI.isEndTrackEvent());
	REQUIRE(tempMIDI.getTickTime() == 100);

}
TEST_CASE("MIDITempoEvent", "[event]") {
	TempoEvent e;
	MIDIEvent tempMIDI(e, 101);
	REQUIRE_FALSE(tempMIDI.isNoteEvent());
	REQUIRE(tempMIDI.isTempoEvent());
	REQUIRE_FALSE(tempMIDI.isEndTrackEvent());
	REQUIRE(tempMIDI.getTickTime() == 101);
}
TEST_CASE("MIDIEndtrackEvent", "[event]") {
	EndTrackEvent e;
	MIDIEvent tempMIDI(e, 99);
	REQUIRE_FALSE(tempMIDI.isNoteEvent());
	REQUIRE_FALSE(tempMIDI.isTempoEvent());
	REQUIRE(tempMIDI.isEndTrackEvent());
	REQUIRE(tempMIDI.getTickTime() == 99);

}



