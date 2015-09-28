#include "onepoledetection.h"
#include <string>
#include <iostream>

using namespace std;

short* ReadDataFromWavFile( const string& fileName, int& sampleCount );

int main()
{
	int sampleCount = 0;
	auto buffer = ReadDataFromWavFile( "123423423.wav", sampleCount );
	auto detector = new OnePoleDetection();
	auto segments(detector->Detect(buffer, sampleCount));


	if (buffer != nullptr)
		free (buffer);


	for (const auto& i : segments)
	{
		cout << i.start << " " << i.end << " " << i.segTime << endl;
	}
	return 0;
}


short* ReadDataFromWavFile(const string& fileName, int& sampleCount)
{
	FILE* fd = fopen(fileName.c_str(), "rb");
	if (fd == nullptr)
	{
		fprintf(stderr, "Cannot open the file!\n");
		exit(-1);
	}
	fseek(fd, 0, SEEK_END);
	unsigned long dataSize = ftell(fd) - 0x2c;
	sampleCount = dataSize / sizeof(short);

	fseek(fd, 0x2c, SEEK_SET);
	short* data = (short*)malloc(dataSize);
	fread(data, sizeof(short), sampleCount, fd);
	fclose(fd);

	return data;
	
}