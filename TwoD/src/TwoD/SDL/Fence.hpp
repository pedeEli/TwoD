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
		Fence(Fence&& other);
		Fence& operator=(const Fence& other) = delete;
		Fence& operator=(Fence&& other) noexcept;

		void Wait() const;

	private:
		Fence(const CommandBuffer* commandBuffer);

	private:
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class CommandBuffer;
	};
}