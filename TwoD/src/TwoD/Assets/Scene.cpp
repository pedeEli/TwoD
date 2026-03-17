#include "tdpch.hpp"
#include "Scene.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/ECS/UITransform.hpp"
#include "Prefab.hpp"


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
			EventHandler::Remove(s_activeScene->m_windowResizedHandle);
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
			m_windowResizedHandle = EventHandler::Add<WindowResizedEvent>([transform](auto& event)
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

	bool Deserializable<ComponentInfo>::Deserialize(const Deserializer& deserializer, ComponentInfo& value)
	{
		if (!deserializer["type"])
		{
			TD_CORE_ERROR("missing field type in ComponentInfo");
			return false;
		}
		if (!deserializer["type"].As<std::string>(value.type))
		{
			return false;
		}
		return ECS::CreateLoadData(value.type, deserializer, value.loadData);
	}

	bool Deserializable<EntityInfo>::Deserialize(const Deserializer& deserializer, EntityInfo& value)
	{
		if (deserializer["prefab"])
		{
			if (!deserializer["prefab"].As<std::string>(value.name))
			{
				return false;
			}
			
			auto& prefab = AssetManager::Get<Prefab>(value.name);
			if (deserializer["name"])
			{
				if (!deserializer["name"].As<std::string>(value.name))
				{
					return false;
				}
			}
			
			return DeserializePrefab(value, prefab.prefab, deserializer);
		}

		if (!deserializer["name"].As<std::string>(value.name))
		{
			return false;
		}


		if (deserializer["components"])
		{
			if (!deserializer["components"].As<std::vector<ComponentInfo>>(value.components))
			{
				return false;
			}
		}

		if (deserializer["transform"])
		{
			if (!ECS::CreateLoadData("class TwoD::UITransform", deserializer["transform"], value.transformLoadData))
			{
				return false;
			}
		}

		if (deserializer["children"])
		{
			if (!deserializer["children"].As<std::vector<EntityInfo>>(value.children))
			{
				return false;
			}
		}

		return true;
	}
	bool Deserializable<EntityInfo>::DeserializePrefab(EntityInfo& value, EntityInfo& prefab, std::optional<Deserializer> deserializer)
	{
		value.transformLoadData = ECS::CopyLoadData("class TwoD::UITransform", prefab.transformLoadData);

		if (deserializer && (*deserializer)["transform"])
		{
			if (!ECS::ModifyLoadData("class TwoD::UITransform", (*deserializer)["transform"], value.transformLoadData))
			{
				return false;
			}
		}

		for (auto& component : prefab.components)
		{
			value.components.emplace_back(component.type, ECS::CopyLoadData(component.type, component.loadData));
		}
		
		if (deserializer && (*deserializer)["components"])
		{
			auto components = (*deserializer)["components"];
			if (!components.IsSequence())
			{
				return false;
			}
			for (size_t i = 0; i < components.GetSize(); i++)
			{
				auto component = components[i];
				std::string type;
				if (!component["type"] || !component["type"].As<std::string>(type))
				{
					return false;
				}
				bool exists = false;
				for (auto& existing : value.components)
				{
					if (existing.type == type)
					{
						ECS::ModifyLoadData(type, component, existing.loadData);
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					ComponentInfo info;
					if (!component.As<ComponentInfo>(info))
					{
						return false;
					}
					value.components.push_back(info);
				}
			}
		}

		std::vector<bool> childIsOnPrefab;
		std::optional<Deserializer> children;
		if (deserializer && (*deserializer)["children"])
		{
			children = (*deserializer)["children"];
			if (!(*children).IsSequence())
			{
				return false;
			}
			childIsOnPrefab.resize(children->GetSize(), false);
		}

		for (auto& prefabChild : prefab.children)
		{
			EntityInfo copy;
			std::optional<Deserializer> child;
			if (children)
			{
				for (size_t i = 0; i < children->GetSize(); i++)
				{
					std::string name;
					if ((*children)[i].As<std::string>(name))
					{
						return false;
					}
					if (prefabChild.name == name)
					{
						child = (*children)[i];
						childIsOnPrefab[i] = true;
						break;
					}
				}
			}
			copy.name = prefabChild.name;
			if (!DeserializePrefab(copy, prefabChild, child))
			{
				return false;
			}
			value.children.push_back(copy);
		}

		if (children)
		{
			for (size_t i = 0; i < children->GetSize(); i++)
			{
				if (childIsOnPrefab[i])
				{
					continue;
				}
				EntityInfo info;
				if ((*children)[i].As<EntityInfo>(info))
				{
					return false;
				}
				value.children.push_back(info);
			}
		}

		return true;
	}

	bool Deserializable<ScreenEntities>::Deserialize(const Deserializer& deserializer, ScreenEntities& value)
	{
		return deserializer.As<std::vector<EntityInfo>>(value.entities);
	}
}