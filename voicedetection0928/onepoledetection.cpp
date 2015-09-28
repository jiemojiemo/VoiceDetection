#include "onepoledetection.h"
#include <algorithm>
using std::vector;

const float OnePoleDetection::kAlpha=0.9;


class OnePoleDetection::Impl
{
public:
	vector<float> OnePoleFilter(const short* buffer, unsigned long sampleCount);
	float CalcThreshold( const vector<float>& energy );
	vector<SpeechSegment> Detect( const vector<float>& energy, float threshold, unsigned long sampleCount);
};

vector<float> OnePoleDetection::Impl::OnePoleFilter(const short * buffer, unsigned long sampleCount)
{
	vector<float> filterOut;
	filterOut.push_back(buffer[0]);
	for (int i = 1; i < sampleCount; ++i)
	{
		filterOut.push_back(buffer[i] + kAlpha*filterOut[i - 1]);
	}
	return filterOut;
}

float OnePoleDetection::Impl::CalcThreshold(const vector<float>& energy)
{
	auto maxEnergy = *max_element(energy.begin(), energy.end());
	return maxEnergy / 6.0;
}

vector<SpeechSegment> OnePoleDetection::Impl::Detect(const vector<float>& energy,float threshold, unsigned long sampleCount)
{
	
	int status = 0;
	int voiceStart = 0;
	int voiceEnd = 0;
	int voiceCount = 0;		//语音计数，大于m_minVoice则认为是语音段
	int silenceCount = 0;	//语音间隔计数，如果大于m_minSilence，表明当前语音段结束
	int minSilence = 1000;	//
	int minLength = 2000;
	vector<SpeechSegment> segments;
	for (int i = 0; i < sampleCount; ++i)
	{
		switch (status)
		{
		case 0:
		case 1:
			if (energy[i] > threshold)
			{
				voiceStart = i;
				status = 2;
				silenceCount = 0;
				voiceCount++;
			}
			else
			{
				status = 0;
				voiceCount = 0;
			}

			break;
		case 2:
			if (energy[i] > threshold)
			{
				voiceCount++;
				silenceCount = 0;
			}
			else
			{
				silenceCount++;
				if (silenceCount < minSilence)
					voiceCount++;
				else if (voiceCount < minLength)
				{
					status = 0;
					silenceCount = 0;
					voiceCount = 0;
				}
				else
					status = 3;		//当前语音段结束，并且语音段长度足够长，说明是一段人声。记录其端点
			}

			break;
		case 3:
			voiceCount = voiceCount - silenceCount / 2;
			voiceEnd = voiceStart + voiceCount - 1;
			segments.push_back(SpeechSegment(voiceStart, voiceEnd));
			status = 0;
			silenceCount = 0;
			voiceCount = 0;
			break;
		}
	}//end for
	return segments;
}


vector<SpeechSegment> OnePoleDetection::Detect(const short* buffer, unsigned long sampleCount)
{
	//Filter
	auto energy(m_pImpl->OnePoleFilter(buffer, sampleCount));
	//Get Threshold 
	auto threshold = m_pImpl->CalcThreshold(energy);
	//Detect
	auto startEndArray = m_pImpl->Detect( energy, threshold, sampleCount );
	//Calc time
	for (auto& i : startEndArray)
	{
		i.segTime = (i.end - i.start) / 44100.0;
	}

	return startEndArray;
}

