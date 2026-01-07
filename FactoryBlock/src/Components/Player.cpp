#include "Player.hpp"
#include <glm/glm.hpp>
#include "Ore.hpp"

void Player::Start()
{
	camera = Camera::Get();
}

void Player::Update(float delta)
{
	auto& input = App::Get<InputSystem>();
	glm::fvec3 dir(0.0f, 0.0f, 0.0f);

	if (input.GetButtonDown(SDLK_A))
	{
		dir.x -= 1.0f;
	}
	if (input.GetButtonDown(SDLK_D))
	{
		dir.x += 1.0f;
	}
	if (input.GetButtonDown(SDLK_W))
	{
		dir.y -= 1.0f;
	}
	if (input.GetButtonDown(SDLK_S))
	{
		dir.y += 1.0f;
	}

	if (dir.x != 0.0 || dir.y != 0.0)
	{
		glm::fvec3 normalized = glm::normalize(dir) * speed * delta;
		GetComponent<Transform>()->Translate(normalized);
	}

	auto wheel = input.GetWheel();
	if (wheel != 0.0f)
	{
		camera->zoom = glm::clamp(camera->zoom - wheel, 5.0f, 50.0f);
	}

	
	if (input.GetMouseDown(1))
	{
		auto& mousePos = camera->GetMousePositionWorld();
		auto hit = Hitbox::Hit(mousePos);
		if (hit && (*hit)->GetEntity()->name == "iron ore")
		{
			(*hit)->GetEntity()->Destroy();
		}
	}
}