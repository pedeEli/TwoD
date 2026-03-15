#include "Dice.hpp"
#include <glm/gtx/compatibility.hpp>

using namespace TwoD;

void Dice::Start()
{
	auto* transform = GetTransform();
	transform->position.x = (2.5f - static_cast<float>(index)) * 1.2f;
	targetPosition = transform->position;
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->layer = 1000;
}

void Dice::Update(float delta)
{
	auto* transform = GetTransform();
	transform->position = glm::lerp(transform->position, targetPosition, speed * delta);

	auto& renderer = *m_renderer;
	renderer.sprite = sprite;
	renderer.color = static_cast<glm::u8vec4>(glm::lerp(
		static_cast<glm::fvec4>(renderer.color),
		targetColor,
		speed * delta
	));
}

void Dice::Toggle()
{
	if (selected)
	{
		targetPosition.y = 0.0f;
		targetColor.a = 255.0f;
		selected = false;
	}
	else
	{
		targetPosition.y = -0.6f;
		targetColor.a = 100.0f;
		selected = true;
	}
}