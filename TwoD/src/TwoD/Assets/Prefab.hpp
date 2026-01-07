#pragma once
//#include <filesystem>
//#include <expected>
//#include <string>
//#include <yaml-cpp/yaml.h>
//
//namespace resources
//{
//	class Prefab
//	{
//	public:
//		static std::expected<Prefab*, std::string> Load(const std::filesystem::path& path, const YAML::Node& file);
//
//	public:
//		Prefab(const engine::EntityInfo& info);
//
//		engine::Entity* Instantiate() const;
//
//	private:
//		engine::EntityInfo m_info;
//	};
//}