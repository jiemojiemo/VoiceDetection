#pragma once
#include <vector>
struct SpeechSegment {
	unsigned int start;	//������ʼ�㣬sample��λ��
	unsigned int end;	//������ֹ�㣬sample��λ��
	float segTime;		//��һ��������ʱ��
	float velocity;		//��һ����������ǿ
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

