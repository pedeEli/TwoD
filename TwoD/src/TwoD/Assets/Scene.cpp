#include "tdpch.hpp"
#include "Scene.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/ECS/Transform.hpp"
#include "TwoD/ECS/UITransform.hpp"


namespace TwoD
{
	static Scene* s_activeScene = nullptr;

	void Scene::LoadEntity(EntityInfo& entityInfo, EntityHandle handle)
	{
		Entity& entity = *handle;

		if (entityInfo.transformLoadData)
		{
			entity.GetComponent<Transform>().Load(entityInfo.transformLoadData);
		}

		for (auto& componentInfo : entityInfo.components)
		{
			auto& component = entity.AddComponent(componentInfo.type);
			if (componentInfo.loadData)
			{
				component.Load(componentInfo.loadData);
			}
		}

		for (auto& childInfo : entityInfo.children)
		{
			auto& childEntity = ECS::CreateEntity(childInfo.name, handle);
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
		}

		for (auto& entityInfo : entities)
		{
			EntityHandle handle = ECS::CreateEntity(entityInfo.name);
			m_rootEntities.push_back(handle);
			LoadEntity(entityInfo, handle);
		}

		if (screen.entities.size() != 0)
		{
			m_screenRootEntity = ECS::CreateUIEntity("screen");
			for (auto& entityInfo : screen.entities)
			{
				auto& entity = ECS::CreateEntity(entityInfo.name, m_screenRootEntity);
				LoadEntity(entityInfo, entity);
			}

			ComponentHandle<UITransform> transform = m_screenRootEntity->GetComponent<UITransform>();
			EventHandler::On<WindowResizedEvent>([transform](auto& event)
				{
					glm::fvec2 size = { static_cast<float>(event.x), static_cast<float>(event.x) };
					transform->SetSize(size);
					transform->SetPosition(size * 0.5f);
					return false;
				});
			auto size = static_cast<glm::fvec2>(App::Get<Window>().GetSize());
			transform->SetSize(size);
			transform->SetPosition(size * 0.5f);
		}

		s_activeScene = this;
	}

	void Scene::UpdateMatrices()
	{
		for (auto entity : m_rootEntities)
		{
			entity->GetTransform()->UpdateMatrix(m_callbacks);
		}
		if (m_screenRootEntity)
		{
			m_screenRootEntity->GetTransform()->UpdateMatrix(m_callbacks);
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