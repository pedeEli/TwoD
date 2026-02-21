#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class Fence
	{
	public:
		Fence();
		~Fence();
		Fence(const Fence& other) = delete;
		Fence(Fence&& other) noexcept;
		Fence& operator=(const Fence& other) = delete;
		Fence& operator=(Fence&& other) noexcept;

		void Release();
		void Wait() const;
		void swap(Fence&& other);

	private:
		Fence(const CommandBuffer* commandBuffer);

	private:
		bool m_released = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class CommandBuffer;
	};
}