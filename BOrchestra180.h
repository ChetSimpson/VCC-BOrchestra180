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


struct IOPorts
{
	static const unsigned int InterfaceControl = 0xe6;
	static const unsigned int MixerControl = 0xe7;
	static const unsigned int Channel1a = 0xe8;
	static const unsigned int Channel1b = 0xe9;	//	Mirror to channal 1
	static const unsigned int Channel2a = 0xea;
	static const unsigned int Channel2b = 0xeb;	//	Mirror to channal 2
	static const unsigned int Channel3a = 0xec;
	static const unsigned int Channel3b = 0xed;	//	Mirror to channal 3
	static const unsigned int Channel4a = 0xee;
	static const unsigned int Channel4b = 0xef;	//	Mirror to channal 4

};

struct ControlBits
{
	struct AllValidBits
	{
		static const unsigned int Mask = 0x7f;
	};

	struct Enable
	{
		static const unsigned int Mask = 0x01;
		static const unsigned int Shift = 0;
	};

	struct IRQType
	{
		static const unsigned int Mask = 0x02;
		static const unsigned int Shift = 1;
	};

	struct BufferSize
	{
		static const unsigned int Mask = 0x0c;
		static const unsigned int Shift = 2;
	};

	struct Clock
	{
		static const unsigned int Mask = 0x70;
		static const unsigned int Shift = 4;
	};

	//	Read only
	struct InterruptPending
	{
		static const unsigned int Value = 0x80;
	};
};

//
//	Register 0 - Control (Default = 0 / disabled)
// 
//		Writing to this register resets the primary state of the buffers including
//		rewinding the playback and writing offsets.
// 
//		Reading from or writing to this register clears any pending interrupts.
// 
//		0		Enable. 0 = disabled, 1 = enabled
//		1		IRQ type. 0 = IRQ, 1 = FIRQ
//		2-3		Buffer size. 0 = 64. 1 = 128, 2 = 192, 3 = 256
//		4-6		Clock.
//					0	1920
//					1	3840
//					2	5760
//					3	7680
//					4	9600
//					5	11520
//					6	13440
//					7	15360
//		7		Interrupt pending (read only)
//
//	Register 1 - Mixer channel select (Default = 0xff / mono)
//
//		0-1	Channel 1 pan level.
//				0	Off
//				1	Full left
//				2	Full Right
//				3	Full left and right
//		2-3	Channel 2 pan level.
//		4-5	Channel 3 pan level.
//		6-7	Channel 4 pan level.
//



//	Common CPU defs from VCC (Why the fuck are these duplicated all over the place?)
#define IRQ		1
#define FIRQ	2
#define NMI		3

#define MAX_LOADSTRING 100

typedef void (*ASSERTINTERUPT) (unsigned char,unsigned char);
typedef void (*DYNAMICMENUCALLBACK)( char *,int, int);
