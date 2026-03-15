#include "tdpch.hpp"
#include "Scene.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/ECS/UITransform.hpp"


namespace TwoD
{
	static Scene* s_activeScene = nullptr;

	void Scene::LoadEntity(EntityInfo& entityInfo, EntityHandle handle)
	{
		Entity& entity = *handle;

		for (auto& componentInfo : entityInfo.components)
		{
			entity.AddComponent(componentInfo.type, componentInfo.loadData);
		}

		for (auto& childInfo : entityInfo.children)
		{
			auto& childEntity = ECS::CreateEntity(childInfo.name, handle, childInfo.transformLoadData);
			LoadEntity(childInfo, childEntity);
		}
	}

	Scene& Scene::GetActive()
	{
		TD_CORE_ASSERT(s_activeScene);
		return *s_activeScene;
	}
	void Scene::AddPropagationCallback(Transform::PropagationCallback callback)
	{
		m_callbacks.push_back(callback);
	}

	void Scene::Destroy()
	{
		for (auto entity : m_rootEntities)
		{
			entity->Destroy();
		}
		if (m_screenRootEntity)
		{
			m_screenRootEntity->Destroy();
		}
	}

	void Scene::SetActive()
	{
		if (s_activeScene)
		{
			for (auto entity : s_activeScene->m_rootEntities)
			{
				entity->Destroy();
			}
			s_activeScene->m_rootEntities.clear();
			if (s_activeScene->m_screenRootEntity)
			{
				s_activeScene->m_screenRootEntity->Destroy();
				s_activeScene->m_screenRootEntity = EntityHandle::None;
			}
			s_activeScene->m_windowResizedHandle.Off();
		}

		for (auto& entityInfo : entities)
		{
			EntityHandle handle = ECS::CreateEntity(entityInfo.name, entityInfo.transformLoadData);
			m_rootEntities.push_back(handle);
			LoadEntity(entityInfo, handle);
		}

		if (screen.entities.size() != 0)
		{
			m_screenRootEntity = ECS::CreateUIEntity("screen", nullptr);
			for (auto& entityInfo : screen.entities)
			{
				auto& entity = ECS::CreateEntity(entityInfo.name, m_screenRootEntity, entityInfo.transformLoadData);
				LoadEntity(entityInfo, entity);
			}

			ComponentHandle<UITransform> transform = m_screenRootEntity->GetComponent<UITransform>();
			auto size = static_cast<glm::fvec2>(App::Get<Window>().GetSize());
			transform->size = size;
			transform->anchor = Anchor::BOTTOM_RIGHT;
			m_windowResizedHandle = EventHandler::On<WindowResizedEvent>([transform](auto& event)
				{
					glm::fvec2 size = { static_cast<float>(event.x), static_cast<float>(event.y) };
					transform->size = size;
					return false;
				});
		}

		s_activeScene = this;
	}

	void Scene::UpdateMatrices()
	{
		for (auto entity : m_rootEntities)
		{
			entity->GetComponent<Transform>().UpdateMatrix(m_callbacks);
		}
		if (m_screenRootEntity)
		{
			m_screenRootEntity->GetComponent<UITransform>().UpdateMatrix(m_callbacks);
		}
	}

	const std::vector<EntityHandle>& Scene::GetRootEntities() const
	{
		return m_rootEntities;
	}
	EntityHandle Scene::GetScreenRootEntity() const
	{
		return m_screenRootEntity;
	}

	bool Deserializable<TwoD::ComponentInfo>::Deserialize(const Deserializer& deserializer, TwoD::ComponentInfo& value)
	{
		if (!deserializer["type"])
		{
			TD_CORE_ERROR("missing field type in TwoD::ComponentInfo");
			return false;
		}
		if (!deserializer["type"].As<std::string>(value.type))
		{
			return false;
		}
		return TwoD::ECS::CreateLoadData(value.type, deserializer, value.loadData);
	}

	bool Deserializable<TwoD::EntityInfo>::Deserialize(const Deserializer& deserializer, TwoD::EntityInfo& value)
	{
		if (!deserializer["name"].As<std::string>(value.name))
		{
			return false;
		}


		if (deserializer["components"])
		{
			if (!deserializer["components"].As<std::vector<TwoD::ComponentInfo>>(value.components))
			{
				return false;
			}
		}

		if (deserializer["transform"])
		{
			if (!TwoD::ECS::CreateLoadData("class TwoD::Transform", deserializer["transform"], value.transformLoadData))
			{
				return false;
			}
		}

		if (deserializer["children"])
		{
			if (!deserializer["children"].As<std::vector<TwoD::EntityInfo>>(value.children))
			{
				return false;
			}
		}

		return true;
	}

	bool Deserializable<TwoD::ScreenEntities>::Deserialize(const Deserializer& deserializer, TwoD::ScreenEntities& value)
	{
		return deserializer.As<std::vector<TwoD::EntityInfo>>(value.entities);
	}
}