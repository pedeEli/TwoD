#pragma once
#include <expected>
#include <optional>
#include <glm/glm.hpp>
#include "TwoD/ECS/ECS.hpp"

namespace TwoD
{
	struct ComponentInfo
	{
		std::string type;
		YAML::Node data;
	};

	struct TransformInfo
	{
		std::optional<glm::fvec2> position;
		std::optional<float> rotation;
		std::optional<glm::fvec2> scale;
	};

	struct EntityInfo
	{
	public:
		static std::expected<EntityInfo, std::string> FromYAML(const YAML::Node& node);

		std::string name;
		std::vector<ComponentInfo> components;
		std::vector<EntityInfo> children;
		TransformInfo transform;
	};

	class Scene
	{
	public:
		Scene(const std::vector<EntityInfo> &entityInfos) : m_entityInfos(entityInfos) {}

		void Load();
		void Unload();

	private:
		Ref<Entity> LoadEntity(ECS& ecs, EntityInfo& entityInfo);

	private:
		std::vector<EntityInfo> m_entityInfos;
	};

	class SceneSystem
	{
	public:
		SceneSystem() = default;
		~SceneSystem() = default;
		SceneSystem(SceneSystem& other) = delete;
		SceneSystem(SceneSystem&& other) = delete;
		SceneSystem& operator=(SceneSystem& other) = delete;
		SceneSystem& operator=(SceneSystem&& other) = delete;

		void Load();

		void SetActive(const std::string& name);

		void Clear();

	private:
		void LoadFile(const std::string& path);

	private:
		std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_activeScene;
	};
}