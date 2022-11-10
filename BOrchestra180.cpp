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
#include "BOrchestra180.h"
#include "Mixer.h"
#include "resource.h" 


namespace
{
	ASSERTINTERUPT VCCAssertInterupt = [](unsigned char, unsigned char) {};

	Mixer BOrchMixer;

	const std::array<Mixer::pan_type, 4> PanConversion =
	{
		Mixer::pan_type::Off,
		Mixer::pan_type::Left,
		Mixer::pan_type::Right,
		Mixer::pan_type::Both
	};

	const std::array<int, 2> IRQTypes = { IRQ, NMI };

	const std::array<float, 8> NTSCClockValues
	{
		1.0f / 8,
		1.0f / 7,
		1.0f / 6,
		1.0f / 5,
		1.0f / 4,
		1.0f / 3,
		1.0f / 2,
		1.0f / 1,
	};

	const std::array<size_t, 4> BufferSizes
	{
		64,
		128,
		192,
		256
	};
	

	namespace PakState
	{
		//	General interface state
		unsigned char ControlBits = 0;
		unsigned char MixerControlBits = 0;
		bool IsEnabled = false;
		int IRQType = IRQ;

		//	IRQ state
		bool IsIRQTriggered = false;
	}
}




extern "C" void ProgramPak_SetAssertInteruptCallback(ASSERTINTERUPT assertInterupt)
{
	VCCAssertInterupt = assertInterupt;
}


extern "C" void ProgramPak_WritePort(unsigned char port, unsigned char data)
{
	switch (port)
	{
	case IOPorts::InterfaceControl:
		PakState::IsIRQTriggered = false;
		//	Stash the control bits for writing and decode them into our state
		PakState::ControlBits = data & ControlBits::AllValidBits::Mask;
		PakState::IsEnabled = (data & ControlBits::Enable::Mask) >> ControlBits::Enable::Shift;
		PakState::IRQType = IRQTypes[(data & ControlBits::IRQType::Mask) >> ControlBits::IRQType::Shift];
		BOrchMixer.SetBufferSize(BufferSizes[(data & ControlBits::BufferSize::Mask) >> ControlBits::BufferSize::Shift]);
		BOrchMixer.SetClockValue(NTSCClockValues[(data & ControlBits::Clock::Mask) >> ControlBits::Clock::Shift]);
		BOrchMixer.Rewind();
		break;


	case IOPorts::MixerControl:
		PakState::MixerControlBits = data;
		BOrchMixer.SetPanLevel(0, PanConversion[data & 0x3]);
		BOrchMixer.SetPanLevel(1, PanConversion[(data >> 2) & 0x3]);
		BOrchMixer.SetPanLevel(2, PanConversion[(data >> 4) & 0x3]);
		BOrchMixer.SetPanLevel(3, PanConversion[(data >> 6) & 0x3]);
		break;

	case IOPorts::Channel1a:
	case IOPorts::Channel1b:
		BOrchMixer.WriteToChannel(0, data);
		break;

	case IOPorts::Channel2a:
	case IOPorts::Channel2b:
		BOrchMixer.WriteToChannel(1, data);
		break;

	case IOPorts::Channel3a:
	case IOPorts::Channel3b:
		BOrchMixer.WriteToChannel(2, data);
		break;

	case IOPorts::Channel4a:
	case IOPorts::Channel4b:
		BOrchMixer.WriteToChannel(3, data);
		break;
	}
}


extern "C" unsigned char ProgramPak_ReadPort(unsigned char port)
{
	unsigned char portByte = 0;

	switch (port)
	{
	case IOPorts::InterfaceControl:
		portByte = PakState::ControlBits;
		if (PakState::IsIRQTriggered)
		{
			PakState::IsIRQTriggered = false;
			portByte |= ControlBits::InterruptPending::Value;
		}
		break;

	case IOPorts::MixerControl:
		portByte = PakState::MixerControlBits;
		break;
	}

	return portByte;
}


// This gets called at the end of every scan line 262 Lines * 60 Frames = 15780 Hz 15720
extern "C" unsigned short ProgramPak_QueryAudioSample()
{
	if (PakState::IsEnabled /*&& !PakState::IsIRQTriggered*/)
	{
		BOrchMixer.Pulse();
		if (BOrchMixer.IsCycleCompleted())
		{
			BOrchMixer.Rewind();
			PakState::IsIRQTriggered = true;
			VCCAssertInterupt(IRQ, 0);
		}
	}

	return BOrchMixer.GetCombindSamples();
}



extern "C" void ProgramPak_Reset(void)
{
	BOrchMixer.Reset();
	ProgramPak_WritePort(IOPorts::InterfaceControl, 0);
}
