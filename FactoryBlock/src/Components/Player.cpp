#include "Player.hpp"
#include <glm/glm.hpp>
#include "Ore.hpp"
#include "Tile.hpp"

using namespace TwoD;

void Player::Start()
{
	camera = Camera::Get();
	EventHandler::On<MouseWheelEvent>([this](auto& e)
		{
			camera->zoom = glm::clamp(camera->zoom - e.y, 5.0f, 50.0f);
			return false;
		});
	EventHandler::On<KeyDownEvent>([this](auto& e)
		{
			if (e.key != Key::SPACE)
			{
				return false;
			}
			auto& renderer = GetComponent<SpriteRenderer>();
			renderer.layer = renderer.layer == 20 ? 10 : 20;

			AssetManager::Get<Audio>("vibraphone").Play();
			return false;
		});
	EventHandler::On<MouseDownEvent>([this](auto& e)
		{
			if (e.button != MouseButton::LEFT)
			{
				return false;
			}

			auto hit = Hitbox::Hit(Camera::Get()->GetMousePositionWorld());
			if (hit && (*hit)->GetEntity().name == "iron ore")
			{
				(*hit)->GetEntity().Destroy();
			}
			else if (hit && (*hit)->GetEntity().name == "tile")
			{
				(*hit)->GetComponent<Tile>().targetRotation += glm::half_pi<float>();
			}
			return false;
		});
}

void Player::Update(float delta)
{
	glm::fvec2 dir(0.0f, 0.0f);

	if (Inputs::GetButtonDown(Scancode::A))
	{
		dir.x -= 1.0f;
	}
	if (Inputs::GetButtonDown(Scancode::D))
	{
		dir.x += 1.0f;
	}
	if (Inputs::GetButtonDown(Scancode::W))
	{
		dir.y -= 1.0f;
	}
	if (Inputs::GetButtonDown(Scancode::S))
	{
		dir.y += 1.0f;
	}

	if (dir.x != 0.0 || dir.y != 0.0)
	{
		auto normalized = glm::normalize(dir) * speed * delta;
		GetComponent<Transform>().position += normalized;
	}
}