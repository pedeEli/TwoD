#pragma once
#include "Defines.hpp"
#include "TwoD/Audio/AudioDefines.hpp"

namespace TwoD::SDL
{
	class AudioStream
	{
	public:
		AudioStream();
		~AudioStream();
		AudioStream(const AudioStream& other) = delete;
		AudioStream(AudioStream&& other) noexcept;
		AudioStream& operator=(const AudioStream& other) = delete;
		AudioStream& operator=(AudioStream&& other) noexcept;

		void Destroy();
		void swap(AudioStream&& other);

		void Play(const AudioSource& source) const;
		int RemainingQueue() const;

	private:
		bool m_destroyed = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class ::TwoD::AudioDevices;
	};
}

