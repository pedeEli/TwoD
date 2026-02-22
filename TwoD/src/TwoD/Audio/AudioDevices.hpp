#pragma once
#include "AudioDefines.hpp"
#include "TwoD/SDL/AudioStream.hpp"
#include "TwoD/SDL/AudioSource.hpp"

namespace TwoD
{
	class AudioDevices
	{
	public:
		static void Init();
		static void Shutdown();

		static SDL::AudioStream GetStream(const SDL::AudioSource& source);

	private:
		static inline uint32_t m_defaultDevice;
	};
}

