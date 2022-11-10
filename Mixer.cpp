//////////////////////////////////////////////////////////////////////////////
//	BOrchestra-180 Software License
// 
//	Copyright 2022 Chet Simpson & HyperTech Gaming
//	
//	Permission to use, copy, modify, and/or distribute this software for any
//  purpose without fee is hereby granted, provided that the above copyright
//  notice, this permission notice, and full credit appear in all copies
//  provided in any and all associated documentation and is visibily
//  displayed in the copyright notice of products that use it.
//	
//	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
//  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
//  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
//  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
//  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
//  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//////////////////////////////////////////////////////////////////////////////
#include "Mixer.h"
#include <cmath>


const float Mixer::DefaultClockValue = 1.0f;


void Mixer::Reset()
{
	for (auto& channel : channels_)
	{
		channel.Reset(logicalBufferSize_);
	}

	logicalBufferSize_ = DefaultBufferSize;
	bufferPosition_ = 0;
	leftChannelSample_ = 0;
	rightChannelSample_ = 0;
	clockValue_ = DefaultClockValue;
}


void Mixer::SetBufferSize(size_t size)
{
	for (auto& channel : channels_)
	{
		channel.SetLogicalSize(size);
	}

	logicalBufferSize_ = size;
	bufferPosition_ = static_cast<float>(std::fmod(bufferPosition_, size));
}


void Mixer::SetClockValue(float clockValue)
{
	clockValue_ = clockValue;
}


void Mixer::SetPanLevel(size_t channel, pan_type pan)
{
	channels_[channel].SetPanLevel(pan);
}


void Mixer::Rewind()
{
	for (auto& channel : channels_)
	{
		channel.Rewind();
	}

	bufferPosition_ = static_cast<float>(std::fmod(bufferPosition_, logicalBufferSize_));
}


void Mixer::Pulse()
{
	SampleMixer leftSample;
	SampleMixer rightSample;

	for (auto& channel : channels_)
	{
		const auto sampleToMix(channel[static_cast<size_t>(bufferPosition_)]);
		switch (channel.GetPanLevel())
		{
		case ChannelBuffer::Pan::Off:
			break;
		
		case ChannelBuffer::Pan::Left:
			leftSample(sampleToMix);
			break;

		case ChannelBuffer::Pan::Right:
			rightSample(sampleToMix);
			break;

		case ChannelBuffer::Pan::Both:
			leftSample(sampleToMix);
			rightSample(sampleToMix);
			break;
		}
	}

	leftChannelSample_ = leftSample;
	rightChannelSample_ = rightSample;
	bufferPosition_ += clockValue_;
}


bool Mixer::IsCycleCompleted() const
{
	return static_cast<size_t>(bufferPosition_) == logicalBufferSize_;
}




void Mixer::WriteToChannel(size_t channel, inputsample_type sample)
{
	if (!channels_[channel].IsBufferFull())
	{
		channels_[channel].Write(sample);
	}
}


unsigned short Mixer::GetCombindSamples() const
{
	return (leftChannelSample_ << 8) | (rightChannelSample_ & 0xff);
}


