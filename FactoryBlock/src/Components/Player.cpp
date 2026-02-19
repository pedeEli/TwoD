#include "Player.hpp"
#include <glm/glm.hpp>
#include "Ore.hpp"

void Player::Start()
{
	camera = Camera::Get();
	TwoD::EventHandler::On<TwoD::MouseWheelEvent>([this](auto& e)
		{
			camera->zoom = glm::clamp(camera->zoom - e.y, 5.0f, 50.0f);
			return false;
		});
	TwoD::EventHandler::On<TwoD::KeyDownEvent>([this](auto& e)
		{
			if (e.key != TwoD::Key::SPACE)
			{
				return false;
			}
			auto& renderer = GetComponent<SpriteRenderer>();
			renderer.layer = renderer.layer == 20 ? 10 : 20;
			App::Get<RenderSystem>().UpdateLayerFor<SpriteRenderer>(renderer.layer);
			return false;
		});
}

void Player::Update(float delta)
{
	glm::fvec3 dir(0.0f, 0.0f, 0.0f);

	if (TwoD::Inputs::GetButtonDown(TwoD::Scancode::A))
	{
		dir.x -= 1.0f;
	}
	if (TwoD::Inputs::GetButtonDown(TwoD::Scancode::D))
	{
		dir.x += 1.0f;
	}
	if (TwoD::Inputs::GetButtonDown(TwoD::Scancode::W))
	{
		dir.y -= 1.0f;
	}
	if (TwoD::Inputs::GetButtonDown(TwoD::Scancode::S))
	{
		dir.y += 1.0f;
	}

	if (dir.x != 0.0 || dir.y != 0.0)
	{
		glm::fvec3 normalized = glm::normalize(dir) * speed * delta;
		GetComponent<Transform>().Translate(normalized);
	}

	
	if (TwoD::Inputs::GetMouseDown(TwoD::MouseButton::LEFT))
	{
		auto& mousePos = camera->GetMousePositionWorld();
		auto hit = Hitbox::Hit(mousePos);
		if (hit && (*hit)->GetEntity().name == "iron ore")
		{
			(*hit)->GetEntity().Destroy();
		}
	}
}