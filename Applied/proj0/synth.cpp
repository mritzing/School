//Matthew Ritzinger 3574 Project0
//Converts a csv into .wav file
//currently all tests pass except test4, even though diff shows no differences in the files
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <cstdlib>
using namespace std;

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


int main(int argc, char *argv[])
{


	//create header struct
	wavHeader header;
	header.Subchunk1Size = 16;    // 16
	header.AudioFormat = 1;      // 1
	header.NumChannels = 1;      // Mono = 1
	header.SampleRate = 44100;       // samples per second, 44100
	header.BitsPerSample = 16;    // 8 bits = 8, 16 bits = 16
	header.ByteRate = header.SampleRate * header.NumChannels * header.BitsPerSample / 8;         // SampleRate * NumChannels * BitsPerSample/8
	header.BlockAlign = header.NumChannels * header.BitsPerSample/8;       // NumChannels * BitsPerSample/8
	//ex : .\synth.exe twinkle.csv tw1inkle.wav
	//checks inputs
	if (argc != 3) {
		std::cerr << "Error: two file arguments expected." << std::endl;
		return EXIT_FAILURE;
	}


	//attempts to open file input
	ifstream csvFile(argv[1]);
	if (!csvFile) {
		std::cerr << "Error: cannot open file " << argv[1] << " for reading." << std::endl;
		return EXIT_FAILURE;
	}


	//opens output file in binary mode
	std::ofstream outstream(argv[2], std::ios::binary);
	if (!outstream) {
		std::cerr << "Error: cannot open file " << argv[2] << " for writing." << std::endl;
		return EXIT_FAILURE;
	}


	string note, amplitude, duration;
	double amplitudeDouble, durationDouble, totalDuration;
	const double pi = 3.141592653589793;
	double noteArray[100];
	double ampArray[100];
	double durationArray[100];
	//probably should move to a single queue vs three arrays, but this implementation works fine for now
	int lineCount = 0;


	//reads in file, calculates duration to be used to figure out NUMSAMPLES
	//adds the three values to seperate arrays to be used later
	while (csvFile.good()) {
		//parse input
		getline(csvFile, note, ',');
		getline(csvFile, amplitude, ',');
		getline(csvFile, duration);

		//trim whitespace
		note.erase(note.find_last_not_of(" \n\r\t") + 1);
		amplitude.erase(amplitude.find_last_not_of(" \n\r\t") + 1);
		duration.erase(duration.find_last_not_of(" \n\r\t") + 1);

		//convert from string to usable data types
		durationDouble = atof(duration.c_str());
		amplitudeDouble = atof(amplitude.c_str());
		char charNote = note[0];

		//end of file check
		if (!csvFile.good()) {
			break;
		}
		//debug statements
		//cout << durationDouble << endl;
		//cout << amplitudeDouble << endl;
		//cout << charNote << endl;



		//checks valid inputs 
		switch (charNote) {
		case 'C':
			noteArray[lineCount] = 261.63;
			break;
		case 'D':
			noteArray[lineCount] = 293.66;
			break;
		case 'E':
			noteArray[lineCount] = 329.63;
			break;
		case 'F':
			noteArray[lineCount] = 349.63;
			break;
		case 'G':
			noteArray[lineCount] = 392.0;
			break;
		case 'A':
			noteArray[lineCount] = 440.0;
			break;
		case 'B':
			noteArray[lineCount] = 493.88;
			break;
		default:
			cerr << "Input note not valid" << endl;
			return EXIT_FAILURE;
			break;
		}


		ampArray[lineCount] = amplitudeDouble;
		durationArray[lineCount] = durationDouble;


		if (durationDouble < 0 || amplitudeDouble < 0) {
			cerr << "Invalid input" << endl;
			return EXIT_FAILURE;
		}

		totalDuration += durationDouble;
		lineCount++;
	}

	//calculates the number of samples to be used later
	int numSamples = totalDuration * header.SampleRate;
	

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

	int16_t dataPoint;
	double delta  = 1.0/double(header.SampleRate);

	//calculates sin function of actual wav data
	for (int i = 0; i < lineCount; i++) {
		int n = int(double(header.SampleRate) * durationArray[i]);
		for (int j = 0; j < n; j++) {
			double time = double(j) * delta;
			dataPoint = ampArray[i] * sin(2 * pi * noteArray[i] * time);
			outstream.write(reinterpret_cast<std::fstream::char_type*>(&dataPoint), sizeof dataPoint);
		}
	}
	return 0;
} 
