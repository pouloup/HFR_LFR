#pragma once

class HFR2LFRYuvReader {
public:
	HFR2LFRYuvReader();
	virtual ~HFR2LFRYuvReader();
	int YuvReader(const char * file, int w, int h);
	//int YuvReader(int argc, char **argv);
};


