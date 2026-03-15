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
		if (deserializer["prefab"])
		{
			if (!deserializer["prefab"].As<std::string>(value.name))
			{
				return false;
			}
			auto& prefab = AssetManager::Get<Prefab>(value.name);
			value.transformLoadData = prefab.prefab.transformLoadData;
			value.components = prefab.prefab.components;
			value.children = prefab.prefab.children;

			if (deserializer["name"])
			{
				if (!deserializer["name"].As<std::string>(value.name))
				{
					return false;
				}
			}

			return Modify(deserializer, value);
		}

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
			if (!TwoD::ECS::CreateLoadData("class TwoD::UITransform", deserializer["transform"], value.transformLoadData))
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
	bool Deserializable<TwoD::EntityInfo>::Modify(const Deserializer& deserializer, TwoD::EntityInfo& value)
	{
		auto transform = deserializer["transform"];
		if (transform)
		{
			if (!ECS::ModifyLoadData("class TwoD::UITransform", transform, value.transformLoadData))
			{
				return false;
			}
		}

		auto components = deserializer["components"];
		if (components)
		{
			if (!components.IsSequence())
			{
				return false;
			}
			for (size_t i = 0; i < components.GetSize(); i++)
			{
				auto component = components[i];
				if (!component["type"])
				{
					TD_CORE_ERROR("missing field type in TwoD::ComponentInfo");
					return false;
				}
				std::string type;
				if (!component["type"].As<std::string>(type))
				{
					return false;
				}
				bool exists = false;
				for (auto& existing : value.components)
				{
					if (existing.type == type)
					{
						exists = true;
						ECS::ModifyLoadData(type, component, existing.loadData);
						break;
					}
				}
				if (!exists)
				{
					ComponentInfo newInfo;
					if (!component.As<ComponentInfo>(newInfo))
					{
						return false;
					}
					value.components.push_back(newInfo);
				}
			}
		}

		auto children = deserializer["children"];
		if (children)
		{
			if (!children.IsSequence())
			{
				return false;
			}
			for (size_t i = 0; i < children.GetSize(); i++)
			{
				auto child = children[i];
				if (!deserializer["name"])
				{
					TD_CORE_ERROR("missing field name in TwoD::EntityInfo");
					return false;
				}
				std::string name;
				if (!deserializer["name"].As<std::string>(name))
				{
					return false;
				}
				bool exists = false;
				for (auto& existing : value.children)
				{
					if (existing.name == name)
					{
						exists = true;
						if (!Modify(child, existing))
						{
							return false;
						}
						break;
					}
				}
				if (!exists)
				{
					EntityInfo newInfo;
					if (!child.As<EntityInfo>(newInfo))
					{
						return false;
					}
					value.children.push_back(newInfo);
				}
			}
		}
		return true;
	}

	bool Deserializable<TwoD::ScreenEntities>::Deserialize(const Deserializer& deserializer, TwoD::ScreenEntities& value)
	{
		return deserializer.As<std::vector<TwoD::EntityInfo>>(value.entities);
	}
}