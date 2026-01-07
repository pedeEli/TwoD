#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class TransferBuffer
	{
	public:
		TransferBuffer();
		TransferBuffer(const Window* window, const TransferBufferInfo& info);
		~TransferBuffer();
		TransferBuffer(TransferBuffer&& other) noexcept;
		TransferBuffer& operator=(TransferBuffer&& other) noexcept;

		template<typename T>
		T* Map(bool cycle) const
		{
			return static_cast<T*>(Map(cycle));
		}

	private:
		void* Map(bool cycle) const;

	private:
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class CopyPass;
	};
}

