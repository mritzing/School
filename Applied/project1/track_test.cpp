/** \file Unit tests for Track Module
*/
#include "catch.hpp"

#include "track.hpp"

TEST_CASE("Track test multiple events", "[track]") {
	//const
	Track testTrack(500000);
	REQUIRE(testTrack.getTicksPerBeat() == 500000);

	testTrack.addNoteEvent(9, 50, 0, 50);
	REQUIRE(!testTrack.emptyList());

	testTrack.addNoteEvent(9, 64, 1, 100);
	
	REQUIRE(testTrack.begin()->asNoteEvent().getNoteNumber() == 50);

	testTrack.popEvent();
	REQUIRE(testTrack.begin()->asNoteEvent().getNoteNumber() == 64);

	testTrack.addTempoEvent(10000, 150);
	testTrack.addEndEvent(200);
	auto it = testTrack.begin();
	it++;
	REQUIRE(it->getTickTime() == 150);
	REQUIRE(it->isTempoEvent());
	it++;
	REQUIRE(it->getTickTime() == 200);
	REQUIRE(it->isEndTrackEvent());
}

TEST_CASE("Track list test", "[track]") {
	TrackListType list_;
	REQUIRE(list_.empty());
	Track testTrack(230320);
	list_.push_back(testTrack);
	REQUIRE(list_.size() == 1);
}