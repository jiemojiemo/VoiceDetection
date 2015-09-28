#pragma once
#include "voicedetection.h"

class OnePoleDetection : public VoiceDetection
{
public:
	virtual std::vector<SpeechSegment> Detect(const short* buffer, unsigned long sampleCount);

private:
	class Impl;
	Impl* m_pImpl;

	static const float kAlpha;
};