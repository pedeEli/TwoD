#include "tdpch.hpp"
#include "Prefab.hpp"

//namespace resources
//{
//	std::expected<Prefab*, std::string> Prefab::Load(const std::filesystem::path& path, const YAML::Node& file)
//	{
//		auto entityInfo = engine::EntityInfo::FromYAML(path.string(), file);
//		if (!entityInfo)
//		{
//			return std::unexpected(entityInfo.error());
//		}
//		return new Prefab(*entityInfo);
//	}
//
//	Prefab::Prefab(const engine::EntityInfo& info) : m_info(info) {}
//
//	engine::Entity* Prefab::Instantiate() const
//	{
//		return new engine::Entity(m_info);
//	}
//}