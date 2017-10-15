#include "wav.hpp"

// copied from milestone 0

bool write_wav(Signal & sound, const std::string & filename)
{

	//normalize signal to 16bit 
	sound.normalize();


	//create header struct
	wavHeader header;
	header.Subchunk1Size = 16;    // 16
	header.AudioFormat = 1;      // 1
	header.NumChannels = 1;      // Mono = 1
	header.SampleRate = 44100;       // samples per second, 44100
	header.BitsPerSample = 16;    // 8 bits = 8, 16 bits = 16
	header.ByteRate = header.SampleRate * header.NumChannels * header.BitsPerSample / 8;         // SampleRate * NumChannels * BitsPerSample/8
	header.BlockAlign = header.NumChannels * header.BitsPerSample / 8;       // NumChannels * BitsPerSample/8
	
	//opens output file in binary mode
	std::ofstream outstream(filename, std::ios::binary);
	if (!outstream) {
		std::cerr << "Error: cannot open file " << filename << " for writing." << std::endl;
		return false;
	}

	int numSamples = sound.size();


	//writes initial header specified in wav format and in header struct
	header.ChunkID[0] = 'R';
	header.ChunkID[1] = 'I';
	header.ChunkID[2] = 'F';
	header.ChunkID[3] = 'F';
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.ChunkID[0]), 4);

	header.Subchunk2Size = numSamples * header.NumChannels * header.BitsPerSample / 8;

	header.ChunkSize = 4 + (8 + header.Subchunk1Size) + (8 + header.Subchunk2Size);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.ChunkSize), sizeof header.ChunkSize);

	header.Format[0] = 'W';
	header.Format[1] = 'A';
	header.Format[2] = 'V';
	header.Format[3] = 'E';
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.Format[0]), 4);

	header.Subchunk1ID[0] = 'f';
	header.Subchunk1ID[1] = 'm';
	header.Subchunk1ID[2] = 't';
	header.Subchunk1ID[3] = ' ';
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.Subchunk1ID[0]), 4);

	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.Subchunk1Size), sizeof header.Subchunk1Size);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.AudioFormat), sizeof header.AudioFormat);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.NumChannels), sizeof header.NumChannels);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.SampleRate), sizeof header.SampleRate);

	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.ByteRate), sizeof header.ByteRate);

	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.BlockAlign), sizeof header.BlockAlign);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.BitsPerSample), sizeof header.BitsPerSample);

	header.Subchunk2ID[0] = 'd';
	header.Subchunk2ID[1] = 'a';
	header.Subchunk2ID[2] = 't';
	header.Subchunk2ID[3] = 'a';
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.Subchunk2ID[0]), 4);

	//size of data array
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&header.Subchunk2Size), sizeof header.Subchunk2Size);

	int16_t val;
	for (size_t i = 0; i < sound.size(); i++) {
		val = sound[i];
		outstream.write(reinterpret_cast<std::fstream::char_type*>(&val), sizeof val);
	}
	return true;
}
