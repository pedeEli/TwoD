#pragma once
#include "SDLDefines.hpp"
#include "TwoD/Audio/AudioDefines.hpp"

namespace TwoD::SDL
{
	class AudioSource
	{
	public:
		static AudioSource LoadWAV(const std::filesystem::path& path);

	public:
		AudioSource();
		~AudioSource();
		AudioSource(const AudioSource& other) = delete;
		AudioSource(AudioSource&& other) noexcept;
		AudioSource& operator=(const AudioSource& other) = delete;
		AudioSource& operator=(AudioSource&& other) noexcept;

		void Free();
		void swap(AudioSource&& other);

	private:
		bool m_freed = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class AudioStream;
		friend class ::TwoD::AudioDevices;
	};
}

