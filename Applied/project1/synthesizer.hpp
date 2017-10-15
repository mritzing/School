/** \file Synthesizer Module
 */
#ifndef SYNTHESIZER_HPP
#define SYNTHESIZER_HPP

#include "track.hpp"
#include "signal.hpp"
#include "instrument.hpp"

/** \fn Synthesize a Track into a Signal.
 * Returned signal is empty on failure.
 * \param track the track to synthesize.
 */
Signal synthesize(Track & track);
//Very short, once Signal and DefaultInstrument classes are defined
#endif
