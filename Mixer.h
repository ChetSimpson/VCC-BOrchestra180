#pragma once
#include "ChannelBuffer.h"


class Mixer
{
public:

	static const size_t DefaultBufferSize = 64;
	static const size_t ChannelCount = 4;
	static const float DefaultClockValue;
	using mixedsample_type = uint16_t;
	using inputsample_type = ChannelBuffer::sample_type;
	using pan_type = ChannelBuffer::Pan;


protected:

	class SampleMixer
	{
	public:

		operator mixedsample_type() const
		{
			return sample;
		}

		void operator()(mixedsample_type sampleToMix)
		{
			if (hasSample)
			{
				sample = (sample + sampleToMix) / 2;
			}
			else
			{
				sample = sampleToMix;
				hasSample = true;
			}
		};


	private:

		bool hasSample = false;
		mixedsample_type sample;
	};


public:

	void Reset();
	void SetClockValue(float clockValue);
	void SetPanLevel(size_t channel, pan_type pan);
	void SetBufferSize(size_t size);
	void Rewind();

	void Pulse();
	bool IsCycleCompleted() const;
	
	void WriteToChannel(size_t channel, inputsample_type sample);

	mixedsample_type GetCombindSamples() const;


private:

	std::array<ChannelBuffer, ChannelCount>	channels_;
	size_t logicalBufferSize_ = DefaultBufferSize;
	float bufferPosition_ = 0.0f;
	float clockValue_ = 1.0f;
	mixedsample_type leftChannelSample_ = 0;
	mixedsample_type rightChannelSample_ = 0;
};