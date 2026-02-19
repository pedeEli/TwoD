#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class Buffer
	{
	public:
		Buffer();
		Buffer(const Window* window, const BufferInfo& info);
		~Buffer();
		Buffer(const Buffer& other) = delete;
		Buffer(Buffer&& other) noexcept;
		Buffer& operator=(const Buffer& other) = delete;
		Buffer& operator=(Buffer&& other) noexcept;

	private:
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class RenderPass;
		friend class CopyPass;
	};
}

