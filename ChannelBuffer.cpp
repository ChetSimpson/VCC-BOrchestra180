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
#include "ChannelBuffer.h"


void ChannelBuffer::Reset(size_t logicalBufferSize)
{
	logicalBufferSize_ = logicalBufferSize;
	currentWritePosition_ = 0;
	pan_ = Pan::Off;
}




void ChannelBuffer::SetPanLevel(Pan pan)
{
	pan_ = pan;
}


ChannelBuffer::Pan ChannelBuffer::GetPanLevel() const
{
	return pan_;
}




void ChannelBuffer::SetLogicalSize(size_t size)
{
	if (size != 64 && size != 128 && size != 192 && size != 256)
	{
		throw std::invalid_argument("Invalid buffer sie. Must be 64, 128, 192, or 128");
	}

	currentWritePosition_ %= size;
	logicalBufferSize_ = size;
}


void ChannelBuffer::Write(sample_type sample)
{
	//	FIXME: This should probably just wrap around (this would eliminate the need
	//	for IsBufferFull and better represent simplified hardware)
	if (currentWritePosition_ >= logicalBufferSize_)
	{
		return /*false*/;
	}

	buffer_[currentWritePosition_++] = sample;
}


bool ChannelBuffer::IsBufferFull() const
{
	return currentWritePosition_ == logicalBufferSize_;
}


void ChannelBuffer::Rewind()
{
	currentWritePosition_ = 0;
}




ChannelBuffer::sample_type ChannelBuffer::operator[](size_t offset) const
{
	if (offset >= logicalBufferSize_)
	{
		throw std::out_of_range("Buffer read out of bounds");
	}

	return buffer_[offset];
}
