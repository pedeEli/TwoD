#include "tdpch.hpp"
#include "Audio.hpp"

#include "TwoD/Audio/AudioDevices.hpp"

namespace TwoD
{
	void Audio::Init(const std::filesystem::path& path, [[maybe_unused]] const Window& window)
	{
		auto filePath = path.parent_path() / file;
		m_source = SDL::AudioSource::LoadWAV(filePath);
		for (auto& stream : m_streams)
		{
			stream = AudioDevices::GetStream(m_source);
		}
	}

	void Audio::Destroy()
	{
		m_source.Free();
		for (auto& stream : m_streams)
		{
			stream.Destroy();
		}
	}

	void Audio::Play()
	{
		auto& stream = m_streams[m_index];
		if (stream.RemainingQueue() == 0)
		{
			stream.Play(m_source);
			m_index = (m_index + 1) % m_streams.size();
		}
	}
}