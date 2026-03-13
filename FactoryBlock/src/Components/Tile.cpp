#include "Tile.hpp"

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
	auto& cityRenderer = city.AddComponent<ColorRenderer>();
	cityRenderer.color.r = 200;
	cityRenderer.color.g = 20;
	cityRenderer.color.b = 100;
	auto* transform = city.GetTransform();
	transform->SetScale(std::sqrt(2.0f), std::sqrt(2.0f));
	transform->SetPosition(0.5f, 0.5f);
	transform->SetRotation(glm::quarter_pi<float>());
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
	pathRenderer.layer = 102;
	pathRenderer.color.r = 150;
	pathRenderer.color.g = 150;
	pathRenderer.color.b = 150;
	auto& pathTransform = path.GetComponent<Transform>();
	pathTransform.SetScale(s_pathScales[index]);
	pathTransform.SetPosition(s_pathPositions[index]);
}