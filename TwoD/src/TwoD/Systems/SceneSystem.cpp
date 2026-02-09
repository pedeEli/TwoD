#include "tdpch.hpp"
#include "SceneSystem.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/ECS/Transform.hpp"

namespace TwoD
{
	void SceneSystem::Load()
	{
		auto& paths = App::GetBasePaths();

		for (const auto& path : paths)
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(path / "scenes"))
			{
				if (entry.is_regular_file() && entry.path().extension().string() == ".yaml")
				{
					LoadFile(entry.path().string());
				}
			}
		}
	}

	void SceneSystem::LoadFile(const std::string& path)
	{
		YAML::Node node = YAML::LoadFile(path);

		if (!node["name"])
		{
			TD_CORE_ERROR("Failed to load scene: {}: missing name field!", path);
			return;
		}

		auto name = node["name"].as<std::string>();

		auto entities = node["entities"];
		if (!entities)
		{
			TD_CORE_ERROR("Failed to load scene: {}: missing entities field!", path);
			return;
		}
		if (!entities.IsSequence())
		{
			TD_CORE_ERROR("Failed to load scene: {}: entities field has to be a list!", path);
			return;
		}

		std::vector<EntityInfo> entityInfos;
		entityInfos.reserve(entities.size());
		for (const auto& entity : entities)
		{
			auto entityInfo = EntityInfo::FromYAML(entity);
			if (entityInfo)
			{
				entityInfos.push_back(*entityInfo);
			}
			else
			{
				TD_CORE_ERROR("Failed to load scene: {}: {}!", path, entityInfo.error());
			}
		}

		m_scenes.emplace(name, entityInfos);
	}

	void SceneSystem::SetActive(const std::string& name)
	{
		if (m_activeScene)
		{
			m_activeScene->Unload();
		}

		auto it = m_scenes.find(name);
		if (it == m_scenes.end())
		{
			TD_CORE_ERROR("Failed to set active scene: {} does not exist!", name);
			return;
		}
		it->second.Load();
		m_activeScene = &it->second;
	}
	void SceneSystem::Clear()
	{
		m_activeScene->Unload();
		m_activeScene = nullptr;
	}

	void Scene::Load()
	{
		for (auto& entityInfo : m_entityInfos)
		{
			LoadEntity(entityInfo);
		}
	}

	EntityHandle Scene::LoadEntity(EntityInfo& entityInfo)
	{
		auto& entity = ECS::CreateEntity(entityInfo.name);
		EntityHandle handle = entity;

		auto& transformInfo = entityInfo.transform;
		auto& transform = entity.GetComponent<Transform>();
		if (transformInfo.position)
		{
			transform.SetPosition(*transformInfo.position);
		}
		if (transformInfo.rotation)
		{
			transform.SetRotation(*transformInfo.rotation);
		}
		if (transformInfo.scale)
		{
			transform.SetScale(*transformInfo.scale);
		}

		for (auto& componentInfo : entityInfo.components)
		{
			auto& component = entity.AddComponent(componentInfo.type);
			component.Load(componentInfo.data);
		}

		for (auto& childInfo : entityInfo.children)
		{
			auto child = LoadEntity(childInfo);
			child->GetComponent<Transform>().SetParent(handle);
		}

		return handle;
	}

	void Scene::Unload()
	{

	}

	std::expected <EntityInfo, std::string> EntityInfo::FromYAML(const YAML::Node& node)
	{
		auto& name = node["name"];
		if (!name)
		{
			return std::unexpected("missing entity.name field");
		}
		EntityInfo info{ name.as<std::string>() };

		auto& components = node["components"];
		if (components)
		{
			if (!components.IsSequence())
			{
				return std::unexpected("entity.components field has to be a list");
			}

			info.components.reserve(components.size());
			for (const auto& component : components)
			{
				auto type = component["type"];
				if (!type)
				{
					return std::unexpected("missing entity.component.type field");
				}
				info.components.emplace_back(type.as<std::string>(), component);
			}
		}

		auto& transform = node["transform"];
		if (transform)
		{
			TransformInfo transformInfo;
			auto& position = transform["position"];
			auto& rotation = transform["rotation"];
			auto& scale = transform["scale"];
			if (position)
			{
				transformInfo.position = position.as<glm::fvec2>();
			}
			if (rotation)
			{
				transformInfo.rotation = rotation.as<float>();
			}
			if (scale)
			{
				transformInfo.scale = scale.as<glm::fvec2>();
			}
			info.transform = transformInfo;
		}

		auto& children = node["children"];
		if (children)
		{
			if (!children.IsSequence())
			{
				return std::unexpected("entity.children field has to be a list");
			}

			info.children.reserve(children.size());
			for (auto& child : children)
			{
				auto childInfo = EntityInfo::FromYAML(child);
				if (childInfo)
				{
					info.children.push_back(*childInfo);
				}
				else
				{
					return std::unexpected(childInfo.error());
				}
			}
		}

		return info;
	}
}