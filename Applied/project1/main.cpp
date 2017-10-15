#include "synthesizer.hpp"
#include "midi.hpp"
#include "wav.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
// TODO: implement the main entry point for synth here

int main(int argc, char *argv[]) {
	//creat synth object read stuff with midi reader
	//write to file with wav
	if (argc != 3) {
		std::cerr << "Error: two file arguments expected." << std::endl;
		return EXIT_FAILURE;
	}
	//attempts to open file input
	//pass filename to midi reader
	std::string outName = argv[2];
	int count = 0;
	TrackListType tracks; //list of all valid tracks of input file
	tracks = readMIDIFromFile(argv[1]); //read track(s) in from MIDI file and store in all_tracks
	bool success = false;
	for (auto & i : tracks) {
		Signal sig = synthesize(i);

		success = write_wav(sig, outName + "-" + std::to_string(count) + ".wav");
		if (!success) {
			std::cerr << "Error: something went wrong in wrtie_wav" << std::endl;
			return EXIT_FAILURE;
		}
		count++;
	}
	return EXIT_SUCCESS;
}
