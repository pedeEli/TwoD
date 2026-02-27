#pragma once
#include <expected>
#include <optional>
#include "AssetDefines.hpp"
#include "AssetManager.hpp"
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

	struct ScreenEntities
	{
	public:
		std::vector<EntityInfo> entities;
	};

	class Scene : public Asset
	{
	public:
		static Scene& GetActive();

	public:
		void Destroy() override;

		void SetActive();
		const std::vector<EntityHandle>& GetRootEntities() const;
		EntityHandle GetScreenRootEntity() const;

		TD_ASSET(
			TD_ASSET_FIELD(std::vector<EntityInfo>, entities),
			TD_ASSET_FIELD(ScreenEntities, screen, {})
		)

	private:
		static void LoadEntity(EntityInfo& entityInfo, EntityHandle handle);

	private:
		std::vector<EntityHandle> m_rootEntities;
		EntityHandle m_screenRootEntity = EntityHandle::None;
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

	template<>
	struct convert<TwoD::ScreenEntities>
	{
		static bool decode(const Node& node, TwoD::ScreenEntities& rhs);
	};
}