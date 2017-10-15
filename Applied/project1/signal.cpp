#include "signal.hpp"

// TODO

Signal::Signal(unsigned int sampleRate)
{
	sampleRate_ = sampleRate;
	delta = 1.0/double(sampleRate_);
}

Signal::~Signal()
{
	delete &sampleList;
}

unsigned int Signal::getSampleRate()
{
	return sampleRate_;
}

void Signal::normalize()
{
	//scale to std::numeric_limits<int16_t>::min() max()
	
	double scaleFactor = 0.0;
	//check if values need to be normalized
	if (std::numeric_limits<int16_t>::min() < sampleMin() || std::numeric_limits<int16_t>::max() > sampleMax()) {
		if (abs(sampleMin()) > abs(sampleMax())) {
			//if negative is greater normalize on negative numbers
			scaleFactor = abs(std::numeric_limits<int16_t>::min() / sampleMin());
		}
		else {
			scaleFactor = std::numeric_limits<int16_t>::max() / sampleMax();
		}
		for (int i = 0; i < size(); i++) {
			sampleList[i] = sampleList[i] * scaleFactor;
		}
	}
}

void Signal::push_back(double value)
{
	sampleList.push_back(value);
}

std::size_t Signal::size()
{
	return sampleList.size();
}

double Signal::operator[](std::size_t index)
{
	return sampleList.at(index);
}



double Signal::sampleMin() 
{
 	auto it = max_element(std::begin(sampleList), std::end(sampleList)); // c++11
 	return *it;
}

double Signal::sampleMax() 
{
	auto it = min_element(std::begin(sampleList), std::end(sampleList)); // c++11
	return *it;
}
