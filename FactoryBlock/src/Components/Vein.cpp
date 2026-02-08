#include "Vein.hpp"
#include <random>
#include "TwoDLib.hpp"

void Vein::Start()
{
	auto& ecs = App::Get<ECS>();
	auto& assetManager = App::Get<AssetManager>();
	auto& iron = assetManager.Get<Sprite>("iron");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distr(-7, 7);

	for (size_t i = 0; i < size; i++)
	{
		int x = distr(gen);
		int y = distr(gen);
		auto& entity = ecs.CreateEntity("iron ore");
		auto& transform = entity.GetComponent<Transform>();
		transform.SetPosition((float)x, (float)y);
		transform.SetParent(&GetEntity());

		auto& renderer = entity.AddComponent<SpriteRenderer>();
		renderer.sprite = &iron;
		renderer.layer = 2;

		auto& hitbox = entity.AddComponent<Hitbox>();
		hitbox.layer = 0;
		hitbox.x = -0.5f;
		hitbox.y = -0.5f;
		hitbox.w = 1.0f;
		hitbox.h = 1.0f;
	}
}