#include "Tile.hpp"

void Tile::Start()
{
	auto& ground = CreateEntity("ground");
	auto& groundRenderer = ground.AddComponent<ColorRenderer>();
	groundRenderer.SetLayer(100);
	groundRenderer.r = 100;
	groundRenderer.g = 200;
	groundRenderer.b = 100;

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
}

void Tile::Update(float delta)
{
	auto* transform = GetTransform();
	auto current = transform->GetRotation();
	transform->SetRotation(std::lerp(current, targetRotation, delta * rotationSpeed));
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
	pathRenderer.SetLayer(101);
	pathRenderer.r = 150;
	pathRenderer.g = 150;
	pathRenderer.b = 150;
	auto& pathTransform = path.GetComponent<Transform>();
	pathTransform.SetScale(s_pathScales[index]);
	pathTransform.SetPosition(s_pathPositions[index]);
}