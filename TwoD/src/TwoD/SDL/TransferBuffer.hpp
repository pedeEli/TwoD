#pragma once
#include "SDLDefines.hpp"

namespace TwoD::SDL
{
	class TransferBuffer
	{
	public:
		TransferBuffer();
		TransferBuffer(const Window* window, const TransferBufferInfo& info);
		~TransferBuffer();
		TransferBuffer(const TransferBuffer& other) = delete;
		TransferBuffer(TransferBuffer&& other) noexcept;
		TransferBuffer& operator=(const TransferBuffer& other) = delete;
		TransferBuffer& operator=(TransferBuffer&& other) noexcept;

		void Release();
		void swap(TransferBuffer&& other);

		template<typename T>
		T* Map(bool cycle) const
		{
			return static_cast<T*>(Map(cycle));
		}

	private:
		void* Map(bool cycle) const;

	private:
		bool m_released = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class CopyPass;
	};
}

