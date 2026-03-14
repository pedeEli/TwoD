#include "Vein.hpp"
#include <random>
#include "TwoDLib.hpp"

using namespace TwoD;

void Vein::Start()
{
	auto& iron = TwoD::AssetManager::Get<Sprite>("iron");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distr(-7, 7);

	for (size_t i = 0; i < size; i++)
	{
		int x = distr(gen);
		int y = distr(gen);
		auto& entity = CreateEntity("iron ore");
		ComponentHandle<Transform> transform = entity.GetComponent<Transform>();
		transform->position.x = static_cast<float>(x);
		transform->position.y = static_cast<float>(y);

		auto& renderer = entity.AddComponent<SpriteRenderer>();
		ComponentHandle<SpriteRenderer> rendererHandle = renderer;
		rendererHandle->sprite = &iron;
		rendererHandle->layer = 2;

		auto& hitbox = entity.AddComponent<Hitbox>();
		hitbox.layer = 0;
		hitbox.x = -0.5f;
		hitbox.y = -0.5f;
		hitbox.w = 1.0f;
		hitbox.h = 1.0f;
	}
}