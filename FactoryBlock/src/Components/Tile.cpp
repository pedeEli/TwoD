#include "Tile.hpp"

using namespace TwoD;

void Tile::Start()
{
	auto& ground = CreateEntity("ground");
	auto& groundRenderer = ground.AddComponent<ColorRenderer>();
	groundRenderer.layer = 100;
	groundRenderer.color.r = 100;
	groundRenderer.color.g = 200;
	groundRenderer.color.b = 100;

	if ((directions & Direction::TOP) != Direction::NONE)
	{
		AddPath("top", Direction::TOP);
	}
	if ((directions & Direction::LEFT) != Direction::NONE)
	{
		AddPath("left", Direction::LEFT);
	}
	if ((directions & Direction::BOTTOM) != Direction::NONE)
	{
		AddPath("bottom", Direction::BOTTOM);
	}
	if ((directions & Direction::RIGHT) != Direction::NONE)
	{
		AddPath("right", Direction::RIGHT);
	}

	auto& city = CreateEntity("city");
	auto& cityRenderer = city.AddComponent<MeshRenderer>();
	cityRenderer.layer = 103;
	cityRenderer.mesh = &AssetManager::Get<Mesh>("triangle");
	cityRenderer.color = { 137, 81, 41, 255 };
	auto* transform = city.GetTransform();
	transform->rotation = glm::pi<float>();
}

void Tile::Update(float delta)
{
	auto* transform = GetTransform();
	transform->rotation = std::lerp(transform->rotation, targetRotation, delta * rotationSpeed);
}


constexpr static glm::fvec2 s_pathScales[4] = {
	{ 0.1f,  0.55f },
	{ 0.55f, 0.1f },
	{ 0.1f,  0.55f },
	{ 0.55f, 0.1f }
};
constexpr static glm::fvec2 s_pathPositions[4] = {
	{  0.0f,   -0.225f },
	{  0.225f,  0.0f },
	{  0.0f,    0.225f },
	{ -0.225f,  0.0f }
};

void Tile::AddPath(const char* name, Direction dir)
{
	auto index = std::bit_width(static_cast<uint8_t>(dir)) - 1;
	auto& path = CreateEntity(std::format("path-{}", name));
	auto& pathRenderer = path.AddComponent<ColorRenderer>();
	pathRenderer.layer = 102;
	pathRenderer.color.r = 150;
	pathRenderer.color.g = 150;
	pathRenderer.color.b = 150;
	auto& pathTransform = path.GetComponent<Transform>();
	pathTransform.scale = s_pathScales[index];
	pathTransform.position = s_pathPositions[index];
}