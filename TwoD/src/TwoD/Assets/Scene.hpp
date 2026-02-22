#pragma once
#include <expected>
#include <optional>
#include "TwoD/Assets/AssetManager.hpp"
#include "TwoD/ECS/ECS.hpp"


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
		static Scene& GetActive();

	public:
		void Destroy() override;

		void SetActive();
		const std::vector<EntityHandle>& GetRootEntities() const;

		TD_ASSET(
			TD_ASSET_FIELD(std::vector<EntityInfo>, entities)
		)

	private:
		std::vector<EntityHandle> m_rootEntities;
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