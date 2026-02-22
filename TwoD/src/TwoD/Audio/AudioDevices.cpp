#include "tdpch.hpp"
#include "AudioDevices.hpp"

#include "TwoD/SDL/Raw/AudioStream.hpp"
#include "TwoD/SDL/Raw/AudioSource.hpp"

namespace TwoD
{
	static SDL_AudioSpec specs{
		.format = SDL_AUDIO_F32,
		.channels = 2,
		.freq = 48000
	};

	void AudioDevices::Init()
	{
		m_defaultDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &specs);
		TD_CORE_ASSERT(m_defaultDevice != 0);
	}

	void AudioDevices::Shutdown()
	{
		SDL_CloseAudioDevice(m_defaultDevice);
	}

	SDL::AudioStream AudioDevices::GetStream(const SDL::AudioSource& source)
	{
		TD_CORE_ASSERT(source.m_raw && !source.m_freed);
		auto* stream = SDL_CreateAudioStream(&source.m_raw->specs, &specs);
		TD_CORE_ASSERT(stream);
		auto success = SDL_BindAudioStream(m_defaultDevice, stream);
		TD_CORE_ASSERT(success, SDL_GetError());

		SDL::AudioStream s;
		s.m_raw = std::make_unique<SDL::AudioStream::Raw>(stream);
		return s;
	}
}