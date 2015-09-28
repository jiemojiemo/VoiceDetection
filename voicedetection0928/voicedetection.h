#pragma once
#include <vector>
struct SpeechSegment {
	unsigned int start;	//人声起始点，sample的位置
	unsigned int end;	//人声终止点，sample的位置
	float segTime;		//这一段人声的时长
	float velocity;		//这一段人声的音强
	SpeechSegment(int s, int e, float b=0, float v=1) :
		 start(s),
		 end(e),
		 segTime(b),
		 velocity(v)
	{
	}
};

class VoiceDetection
{
public:
	virtual std::vector<SpeechSegment> Detect(const short* buffer, unsigned long sampleCount) = 0;
};

