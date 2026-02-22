#include "tdpch.hpp"
#include "AudioSource.hpp"

#include "Raw/AudioSource.hpp"

namespace TwoD::SDL
{
	AudioSource AudioSource::LoadWAV(const std::filesystem::path& path)
	{
		SDL_AudioSpec specs;
		uint8_t* buffer;
		uint32_t length;
		bool success = SDL_LoadWAV(path.string().c_str(), &specs, &buffer, &length);
		TD_CORE_ASSERT(success, SDL_GetError());

		AudioSource source;
		source.m_raw = std::make_unique<Raw>(specs, buffer, length);
		return source;
	}

	AudioSource::AudioSource() = default;
	AudioSource::~AudioSource()
	{
		TD_CORE_ASSERT(!m_raw || m_freed);
	}

	void AudioSource::Free()
	{
		TD_CORE_ASSERT(!m_freed);
		m_freed = true;
		if (m_raw)
		{
			SDL_free(m_raw->buffer);
		}
	}

	void AudioSource::swap(AudioSource&& other)
	{
		std::swap(m_raw, other.m_raw);
		std::swap(m_freed, other.m_freed);
	}

	AudioSource::AudioSource(AudioSource&& other) noexcept
	{
		swap(std::move(other));
	}
	AudioSource& AudioSource::operator=(AudioSource&& other) noexcept
	{
		if (this != &other)
		{
			swap(std::move(other));
		}
		return *this;
	}
}