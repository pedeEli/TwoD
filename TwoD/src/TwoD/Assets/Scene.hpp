#pragma once
#include <expected>
#include <optional>
#include "AssetDefines.hpp"
#include "AssetManager.hpp"
#include "TwoD/ECS/ECS.hpp"
#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Events/EventHandler.hpp"


namespace TwoD
{
	/* TODO: loadData is not being deleted */
	struct ComponentInfo
	{
		std::string type;
		void* loadData = nullptr;
	};

	struct EntityInfo
	{
	public:
		std::string name;
		void* transformLoadData = nullptr;
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
		static void AddPropagationCallback(Transform::PropagationCallback callback);

	public:
		void Destroy() override;

		void SetActive();
		const std::vector<EntityHandle>& GetRootEntities() const;
		EntityHandle GetScreenRootEntity() const;

		void UpdateMatrices();

		TD_ASSET(
			TD_ASSET_FIELD(std::vector<EntityInfo>, entities),
			TD_ASSET_FIELD(ScreenEntities, screen, {})
		)

	private:
		static void LoadEntity(EntityInfo& entityInfo, EntityHandle handle);

	private:
		std::vector<EntityHandle> m_rootEntities;
		EntityHandle m_screenRootEntity = EntityHandle::None;
		EventHandler::Handle m_windowResizedHandle;

		static inline std::vector<Transform::PropagationCallback> m_callbacks;
	};

	
	template<>
	struct Deserializable<TwoD::ComponentInfo>
	{
		static bool Deserialize(const Deserializer& deserializer, TwoD::ComponentInfo& rhs);
	};

	template<>
	struct Deserializable<TwoD::EntityInfo>
	{
		static bool Deserialize(const Deserializer& deserializer, TwoD::EntityInfo& rhs);
		static bool Modify(const Deserializer& deserializer, TwoD::EntityInfo& value);
	};

	template<>
	struct Deserializable<TwoD::ScreenEntities>
	{
		static bool Deserialize(const Deserializer& deserializer, TwoD::ScreenEntities& rhs);
	};
}