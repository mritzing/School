// DO NOT EDIT
/** \file Unit tests for MIDI Module
*/
#include "catch.hpp"

#include <fstream>

#include "test_config.hpp"

#include "midi.hpp"

TEST_CASE( "Test MIDI parser - no such file", "[midi]" ) {

  std::string fname = TEST_FILE_DIR + "/nosuchfile.mid";
  
  auto tracks = readMIDIFromFile(fname);
    
  REQUIRE(tracks.size() == 0);
}

TEST_CASE( "Test MIDI parser - test0", "[midi]" ) {

  std::string fname = TEST_FILE_DIR + "/test0.mid";
  
  auto tracks = readMIDIFromFile(fname);
    
  REQUIRE(tracks.size() == 1);
}

TEST_CASE( "Test MIDI parser - Wikipedia Example", "[midi]" ) {

  std::string fname = TEST_FILE_DIR + "/MIDI_sample.mid";
  
  auto tracks = readMIDIFromFile(fname);
    
  REQUIRE(tracks.size() == 6);
}
