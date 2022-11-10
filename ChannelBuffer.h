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
#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>


class ChannelBuffer
{
public:

	enum class Pan
	{
		Off = 0,
		Left = 1,
		Right = 2,
		Both = 3
	};

	using sample_type = uint8_t;


public:

	void Reset(size_t logicalBufferSize);

	void SetPanLevel(Pan pan);
	Pan GetPanLevel() const;

	//	Buffer write functionaliry
	void SetLogicalSize(size_t size);
	void Write(sample_type sample);
	bool IsBufferFull() const;
	void Rewind();

	sample_type operator[](size_t offset) const;


private:

	std::array<sample_type, 256>	buffer_;
	size_t currentWritePosition_ = 0;
	size_t logicalBufferSize_ = 0;
	Pan pan_ = Pan::Off;
};
