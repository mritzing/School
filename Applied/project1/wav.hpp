/** \file WAV Module
 */

#ifndef WAV_HPP
#define WAV_HPP

#include <string>
#include "signal.hpp"
#include <iostream>
#include <fstream>

/** \fn Write a signal to a WAV file. Returns true on success, false on failure.
 * The signal should be quantized into a 16 bit integer using rounding to nearest.
 * \param sound the signal to write.
 * \param filename the filename to write to.
 */
bool write_wav(Signal & sound, const std::string &filename);

struct wavHeader {
	int8_t   ChunkID[4];        // "RIFF"
	int32_t  ChunkSize;         // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	int8_t   Format[4];         // "WAVE"
	int8_t   Subchunk1ID[4];   // "fmt "
	int32_t  Subchunk1Size;    // 16
	int16_t  AudioFormat;      // 1
	int16_t  NumChannels;      // Mono = 1
	int32_t  SampleRate;       // samples per second, 44100
	int32_t  ByteRate;         // SampleRate * NumChannels * BitsPerSample/8
	int16_t  BlockAlign;       // NumChannels * BitsPerSample/8
	int16_t  BitsPerSample;    // 8 bits = 8, 16 bits = 16
	int8_t   Subchunk2ID[4];   // "data"
	int32_t  Subchunk2Size;    // NumSamples * NumChannels * BitsPerSample/8
							   //int16_t  Data[NUMSAMPLES]; // the Pulse Code Modulated data
};
#endif
