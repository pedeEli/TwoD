#pragma once
#include <expected>
#include <optional>
#include "TwoD/Assets/AssetManager.hpp"


namespace TwoD
{
	/* TODO: loadData is not being deleted */
	struct ComponentInfo
	{
		std::string type;
		const void* loadData;
	};

	struct EntityInfo
	{
	public:
		std::string name;
		const void* transformLoadData = nullptr;
		std::vector<ComponentInfo> components;
		std::vector<EntityInfo> children;
	};

	class Scene : public Asset
	{
	public:
		void SetActive();

		std::vector<EntityInfo> entities; void Load(const YAML::Node& node) override {
			{
				if (!(node["entities"])) {
					::TwoD::Log::GetCoreLogger()->error("Assertion '{}' failed at {}:{}", "node[\"entities\"]", std::filesystem::path("D:\\c++\\repos\\Game\\TwoD\\src\\TwoD\\Assets\\Scene.hpp").filename().string(), 32); __debugbreak();
				}
			}; entities = node["entities"].as<std::vector<EntityInfo>>();
		}
	};
}

namespace YAML
{
	template<>
	struct convert<TwoD::ComponentInfo>
	{
		static bool decode(const Node& node, TwoD::ComponentInfo& rhs);
	};

	template<>
	struct convert<TwoD::EntityInfo>
	{
		static bool decode(const Node& node, TwoD::EntityInfo& rhs);
	};
}