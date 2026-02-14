#include "tdpch.hpp"
#include "Scene.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/ECS/Transform.hpp"


namespace TwoD
{
	static EntityHandle LoadEntity(EntityInfo& entityInfo)
	{
		auto& entity = ECS::CreateEntity(entityInfo.name);
		EntityHandle handle = entity;

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
			auto child = LoadEntity(childInfo);
			child->GetComponent<Transform>().SetParent(handle);
		}

		return handle;
	}

	void Scene::SetActive()
	{
		ECS::Destroy();

		for (auto& entityInfo : entities)
		{
			LoadEntity(entityInfo);
		}
	}
}

namespace YAML
{
	bool convert<TwoD::EntityInfo>::decode(const Node& node, TwoD::EntityInfo& rhs)
	{
		if (!node["name"])
		{
			TD_CORE_ERROR("missing field name in entity info");
			return false;
		}
		rhs.name = node["name"].as<std::string>();

		if (node["components"])
		{
			rhs.components = node["components"].as<std::vector<TwoD::ComponentInfo>>();
		}

		if (node["transform"])
		{
			rhs.transformLoadData = TwoD::ECS::CreateLoadData("class TwoD::Transform", node["transform"]);
		}

		if (node["children"])
		{
			rhs.children = node["children"].as<std::vector<TwoD::EntityInfo>>();
		}

		return true;
	}

	bool convert<TwoD::ComponentInfo>::decode(const Node& node, TwoD::ComponentInfo& rhs)
	{
		if (!node["type"])
		{
			TD_CORE_ERROR("missing field type in TwoD::ComponentInfo");
			return false;
		}
		rhs.type = node["type"].as<std::string>();
		rhs.loadData = TwoD::ECS::CreateLoadData(rhs.type, node);
		return true;
	}
}