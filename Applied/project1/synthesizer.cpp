#include "synthesizer.hpp"

// TODO

Signal synthesize(Track & track)
{
	DefaultInstrument instrument(track);
	Signal signal(44100);
	while (!instrument.halted()) {
		signal.push_back(instrument.sample(44100));
	}
	return signal;
}