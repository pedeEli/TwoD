#include "Ground.hpp"
#include <random>
#include "TwoDLib.hpp"

using namespace TwoD;

void Ground::Start()
{
	float left = -(float)width * 0.5f;
	float top = -(float)height * 0.5f;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			float x = left + j;
			float y = top + i;

			auto& ground = CreateEntity("ground");
			auto& transform = ground.GetComponent<Transform>();
			transform.position = { x, y };

			auto& renderer = ground.AddComponent<SpriteRenderer>();
			renderer.sprite = &TwoD::AssetManager::Get<Sprite>("grass");
			renderer.layer = 0;
		}
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution distrX(left + 1.0f, -left - 1.0f);
	std::uniform_real_distribution distrY(top + 1.0f, -top - 1.0f);
	std::uniform_int_distribution distrS(0, 15);

	for (size_t i = 0; i < foliage; i++)
	{
		auto x = distrX(gen);
		auto y = distrY(gen);
		auto& entity = CreateEntity("foliage");
		auto& transform = entity.GetComponent<Transform>();
		transform.position = { x, y };
		
		auto& renderer = entity.AddComponent<SpriteRenderer>();
		renderer.slice = distrS(gen);
		renderer.sprite = &TwoD::AssetManager::Get<Sprite>("foliage");
		renderer.layer = 1;
	}
}