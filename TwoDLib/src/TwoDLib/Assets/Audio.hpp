#pragma once
#include "TwoD/Assets/AssetManager.hpp"
#include "TwoD/SDL/AudioSource.hpp"
#include "TwoD/SDL/AudioStream.hpp"


namespace TwoD
{
	class Audio : public Asset
	{
	public:
		void Init(const std::filesystem::path& path, const Window& window) override;
		void Destroy() override;

		void Play();

	public:
		TD_ASSET(
			TD_ASSET_FIELD(std::string, file)
		)

	private:
		SDL::AudioSource m_source;
		std::array<SDL::AudioStream, 10> m_streams;
		uint8_t m_index = 0;
	};
}