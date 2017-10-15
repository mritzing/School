/** \file The DefaultInstrument Module
 */
#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include "event.hpp"
#include "track.hpp"
#include <map>
#include <math.h>

#define PI_ 3.14159265358979323846  

 /** The default instrument. A simple fixed length note instrument using a
  * a sinusoid weighted by an envelope function.
  */
class DefaultInstrument {
public:

	/** No default constructor. A DefaultInstrument must have an associated
	 * track.
	 */
	DefaultInstrument() = delete;

	/** Cannot be copied.
	 */
	DefaultInstrument(const DefaultInstrument &) = delete;

	/** Cannot be assigned.
	 */
	DefaultInstrument & operator=(const DefaultInstrument &) = delete;
	/** Construct a Default Instrument using the specified track.
	 * \param track the track to read events from.
	 */
	DefaultInstrument(const Track & track);

	~DefaultInstrument();

	/** Returns true of the DefaultInstrument has stopped processing events.
	 * This can occu e.g. if all events have been consumed or a EndTrackEvent
	 * has been processed.
	 */
	bool halted();

	/** Return the sample after the (real) time increment. Samples the
	 * instrument based on its state after processing all events prior
	 * to this time.
	 * \param deltaT the time increment in seconds.
	 */
	 // call this repeatedly until hatled is assigned true
	 // This is where Signal Samples are written *see diagram
	 // have to add/remove notes from summation 
	double sample(double deltaT);
	//MIDI time is represented by a tick integer
	// T = microsconds per beat (defined by tempo)
	// M = MIDI clock tiks per beat (defined in MIDI file)
	// T' = (T/10^6)     seconds per beat 
	// want seconds per MIDI clock tick: T'/M
	//Process any new Track events that became active since the last sample time
	//Sum the waveforms for the currently active notes
	//Must maintain a container of notes active at the current time
	//must hold: note number, note velocity, note duration, time turned on
	//use a track iterator to process all events up to the current sample time
	//probably want to define private helper functions to :
	//convert a note number to a frequency
	//determine length of a note
	//evaluate the envelope function
	//sum the active notes
private:
	double envelope(double t);
	double convertFreq(int8_t index);
	struct noteStruct {
		double velocity;
		double timeTurnedOn;
		double duration;
	};
	double currentTempo;
	int32_t numSamples;
	//key is notenumber 
	std::map<int8_t,noteStruct> currentEvents;
	/*State:
		container of active notes
		for each active note: note number, time turned on, velocity
		//use map with notenumber as key
		event iterator 
		sample time
	*/
	Track track_;
	bool halted_;
	double endTrackTime;
};


#endif
