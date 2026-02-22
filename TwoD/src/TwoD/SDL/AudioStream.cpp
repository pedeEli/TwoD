#include "tdpch.hpp"
#include "AudioStream.hpp"

#include "Raw/AudioStream.hpp"
#include "Raw/AudioSource.hpp"

namespace TwoD::SDL
{
	AudioStream::AudioStream() = default;
	AudioStream::~AudioStream()
	{
		TD_CORE_ASSERT(!m_raw || m_destroyed);
	}

	void AudioStream::Destroy()
	{
		TD_CORE_ASSERT(!m_destroyed);
		m_destroyed = true;
		if (m_raw)
		{
			SDL_DestroyAudioStream(m_raw->stream);
		}
	}

	void AudioStream::Play(const AudioSource& source) const
	{
		TD_CORE_ASSERT(m_raw && !m_destroyed);
		TD_CORE_ASSERT(source.m_raw && !source.m_freed);
		bool success = SDL_PutAudioStreamData(m_raw->stream, source.m_raw->buffer, source.m_raw->length);
		TD_CORE_ASSERT(success, SDL_GetError());
		success = SDL_FlushAudioStream(m_raw->stream);
		TD_CORE_ASSERT(success, SDL_GetError());
	}

	int AudioStream::RemainingQueue() const
	{
		TD_CORE_ASSERT(m_raw && !m_destroyed);
		auto queue = SDL_GetAudioStreamQueued(m_raw->stream);
		TD_CORE_ASSERT(queue != -1);
		return queue;
	}

	void AudioStream::swap(AudioStream&& other)
	{
		std::swap(m_raw, other.m_raw);
		std::swap(m_destroyed, other.m_destroyed);
	}

	AudioStream::AudioStream(AudioStream&& other) noexcept
	{
		swap(std::move(other));
	}
	AudioStream& AudioStream::operator=(AudioStream&& other) noexcept
	{
		if (this != &other)
		{
			swap(std::move(other));
		}
		return *this;
	}
}