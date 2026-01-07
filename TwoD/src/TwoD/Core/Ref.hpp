#pragma once
#include <vector>
#include "TwoD/Core/Base.hpp"

namespace TwoD
{
	template<typename T>
	class Ref
	{
	public:
		Ref() {}
		~Ref() {}
		Ref(std::vector<T>* vector, size_t index) : m_vector(vector), m_index(index) {}
		Ref(const Ref<T>& other) noexcept = default;
		Ref(Ref<T>&& other) noexcept = default;

		Ref<T>& operator=(const Ref<T>& other) noexcept = default;
		Ref<T>& operator=(Ref<T>&& other) noexcept = default;

		T* operator->()
		{
			TD_CORE_ASSERT(m_vector != nullptr)
			TD_CORE_ASSERT(m_index < m_vector->size())
			return &m_vector->operator[](m_index);
		}
		const T* operator->() const
		{
			TD_CORE_ASSERT(m_vector != nullptr)
			TD_CORE_ASSERT(m_index < m_vector->size())
			return &m_vector->operator[](m_index);
		}

		T& operator*()
		{
			TD_CORE_ASSERT(m_vector != nullptr)
			TD_CORE_ASSERT(m_index < m_vector->size())
			return m_vector->operator[](m_index);
		}
		const T& operator*() const
		{
			TD_CORE_ASSERT(m_vector != nullptr)
			TD_CORE_ASSERT(m_index < m_vector->size())
			return m_vector->operator[](m_index);
		}

		operator bool() const
		{
			return m_vector != nullptr;
		}

	private:
		std::vector<T>* m_vector = nullptr;
		size_t m_index = 0;
	};
}

