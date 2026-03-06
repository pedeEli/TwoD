#pragma once
#include "SerializationDefines.hpp"
#include "Deserializer.hpp"
#include "TwoD/Generated/ApplyEachMacros.hpp"

#include <glm/glm.hpp>

#define TDI_DESERIALIZABLE_GLM_VEC_IF(x) && deserializer[#x]
#define TDI_DESERIALIZABLE_GLM_VEC_SET(x) if(!deserializer[#x].As<T>(value.x)) { return false; }
#define TDI_DESERIALIZABLE_GLM_VEC_UNION(x, ...) \
	if (deserializer[#x] __VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_DESERIALIZABLE_GLM_VEC_IF, __VA_ARGS__))) { \
		TDI_DESERIALIZABLE_GLM_VEC_SET(x) \
		__VA_OPT__(TD_APPLY_EACH_CONCAT(TDI_DESERIALIZABLE_GLM_VEC_SET, __VA_ARGS__)) \
		return true; \
	}
#define TDI_DESERIALIZABLE_GLM_VEC_SEQUENCE(size) \
	if (deserializer.IsSequence()) { \
		if (deserializer.GetSize() != size) { \
			return false; \
		} \
		for (size_t i = 0; i < size; i++) { \
			if (!deserializer[i].As<T>(value[static_cast<glm::vec<size, T, glm::defaultp>::length_type>(i)])) { \
				return false; \
			} \
		} \
		return true; \
	}
#define TDI_DESERIALIZABLE_GLM_VEC(size, u1, u2, u3) \
	template<typename T> \
	struct Deserializable<glm::vec<size, T, glm::defaultp>> \
	{ \
		static bool Deserialize(const Deserializer& deserializer, glm::vec<size, T, glm::defaultp>& value) \
		{ \
			TDI_DESERIALIZABLE_GLM_VEC_SEQUENCE(size) \
			TDI_DESERIALIZABLE_GLM_VEC_UNION u1 \
			TDI_DESERIALIZABLE_GLM_VEC_UNION u2 \
			TDI_DESERIALIZABLE_GLM_VEC_UNION u3 \
			return false; \
		} \
	};

namespace TwoD
{
	TDI_DESERIALIZABLE_GLM_VEC(1, (x), (r), (s))
	TDI_DESERIALIZABLE_GLM_VEC(2, (x, y), (r, g), (s, t))
	TDI_DESERIALIZABLE_GLM_VEC(3, (x, y, z), (r, g, b), (s, t, p))
	TDI_DESERIALIZABLE_GLM_VEC(4, (x, y, z, w), (r, g, b, a), (s, t, p, q))

	template<typename T>
	struct Deserializable<std::vector<T>>
	{
		static bool Deserialize(const Deserializer& deserializer, std::vector<T>& value)
		{
			if (!deserializer.IsSequence())
			{
				return false;
			}
			auto size = deserializer.GetSize();
			value.reserve(size);
			for (size_t i = 0; i < size; i++)
			{
				auto& v = value.emplace_back();
				if (!deserializer[i].As<T>(v))
				{
					return false;
				}
			}
			return true;
		}
	};

	template<typename T>
	struct Deserializable<std::optional<T>>
	{
		static bool Deserialize(const Deserializer& deserializer, std::optional<T>& value)
		{
			if (deserializer.IsNull())
			{
				value.reset();
				return true;
			}
			T v;
			if (deserializer.As<T>(v))
			{
				value = std::move(v);
				return true;
			}
			return false;
		}
	};

	template<>
	struct Deserializable<int8_t>
	{
		static bool Deserialize(const Deserializer& deserializer, int8_t& value)
		{
			return deserializer.GetInt8(value);
		}
	};
	template<>
	struct Deserializable<int16_t>
	{
		static bool Deserialize(const Deserializer& deserializer, int16_t& value)
		{
			return deserializer.GetInt16(value);
		}
	};
	template<>
	struct Deserializable<int32_t>
	{
		static bool Deserialize(const Deserializer& deserializer, int32_t& value)
		{
			return deserializer.GetInt32(value);
		}
	};
	template<>
	struct Deserializable<int64_t>
	{
		static bool Deserialize(const Deserializer& deserializer, int64_t& value)
		{
			return deserializer.GetInt64(value);
		}
	};

	template<>
	struct Deserializable<uint8_t>
	{
		static bool Deserialize(const Deserializer& deserializer, uint8_t& value)
		{
			return deserializer.GetUInt8(value);
		}
	};
	template<>
	struct Deserializable<uint16_t>
	{
		static bool Deserialize(const Deserializer& deserializer, uint16_t& value)
		{
			return deserializer.GetUInt16(value);
		}
	};
	template<>
	struct Deserializable<uint32_t>
	{
		static bool Deserialize(const Deserializer& deserializer, uint32_t& value)
		{
			return deserializer.GetUInt32(value);
		}
	};
	template<>
	struct Deserializable<uint64_t>
	{
		static bool Deserialize(const Deserializer& deserializer, uint64_t& value)
		{
			return deserializer.GetUInt64(value);
		}
	};

	template<>
	struct Deserializable<float>
	{
		static bool Deserialize(const Deserializer& deserializer, float& value)
		{
			return deserializer.GetFloat(value);
		}
	};
	template<>
	struct Deserializable<double>
	{
		static bool Deserialize(const Deserializer& deserializer, double& value)
		{
			return deserializer.GetDouble(value);
		}
	};

	template<>
	struct Deserializable<bool>
	{
		static bool Deserialize(const Deserializer& deserializer, bool& value)
		{
			return deserializer.GetBool(value);
		}
	};

	template<>
	struct Deserializable<std::string>
	{
		static bool Deserialize(const Deserializer& deserializer, std::string& value)
		{
			return deserializer.GetString(value);
		}
	};
}