#pragma once
#include "AssetDefines.hpp"
#include "TwoD/SDL/Window.hpp"
#include "TwoD/Serialization/Serialization.hpp"

namespace TwoD
{
	class Asset
	{
	public:
		Asset() = default;
		virtual ~Asset() = default;
		Asset(const Asset& other) = delete;
		Asset(Asset&& other) = delete;
		Asset& operator=(const Asset& other) = delete;
		Asset& operator=(Asset&& other) = delete;

		virtual void Load(const Deserializer& deserializer) = 0;
		virtual void Init(const std::filesystem::path& path, const Window& window) {}
		virtual void Destroy() {}
	};

	template<class T>
	requires(std::is_base_of_v<Asset, T>)
	struct Deserializable<T*>
	{
		static bool Deserialize(const Deserializer& deserializer, T*& value);
	};
}