#pragma once
#include <string_view>
#include <string>
#include <concepts>
#include "SerializationDefines.hpp"

namespace TwoD
{
	class Deserializer
	{
	public:
		Deserializer(const std::string& name, const char* input);
		~Deserializer();
		Deserializer(const Deserializer& other) noexcept;
		Deserializer(Deserializer&& other) noexcept;
		Deserializer& operator=(const Deserializer& other) noexcept;
		Deserializer& operator=(Deserializer&& other) noexcept;

		template<typename T>
		bool As(T& value) const
		{
			return Deserializable<T>::Deserialize(*this, value);
		}

		Deserializer operator[](const std::string_view key) const;
		Deserializer operator[](const char* key) const;
		Deserializer operator[](size_t index) const;
		operator bool() const;

		bool IsNull() const;
		bool IsSequence() const;
		size_t GetSize() const;

		bool GetInt8(int8_t& value) const;
		bool GetInt16(int16_t& value) const;
		bool GetInt32(int32_t& value) const;
		bool GetInt64(int64_t& value) const;

		bool GetUInt8(uint8_t& value) const;
		bool GetUInt16(uint16_t& value) const;
		bool GetUInt32(uint32_t& value) const;
		bool GetUInt64(uint64_t& value) const;

		bool GetFloat(float& value) const;
		bool GetDouble(double& value) const;

		bool GetBool(bool& value) const;

		bool GetString(std::string& value) const;

	private:
		Deserializer(const std::string& name);

	private:
		struct Raw;
		std::shared_ptr<Raw> m_raw;
	};
}