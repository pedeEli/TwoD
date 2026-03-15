#include "Dices.hpp"

using namespace TwoD;

void Dices::Start()
{
	std::random_device rd;
	m_gen = std::mt19937(rd());

	for (size_t i = 0; i < 6; i++)
	{
		m_sprites[i] = &AssetManager::Get<Sprite>(std::format("dice-{}", i + 1));

		auto& entity = CreateEntity(std::format("dice-{}", i));
		m_dices[i] = entity.AddComponent<Dice>();
		m_dices[i]->index = static_cast<uint8_t>(i);
		entity.AddComponent<Hitbox>();
	}

	Roll();
	m_rollButton = GetEntity("roll")->GetComponent<Button>();
	m_rollHandle = m_rollButton->mouseClick.Add([this]()
		{
			Roll();
		});

	m_mouseDownHandle = EventHandler::Add<MouseDownEvent>([](auto& event)
		{
			auto pos = Camera::Get()->ScreenToWorldSpace({ event.x, event.y });
			auto hit = Hitbox::Hit(pos);
			Dice* dice = nullptr;
			if (hit && (dice = hit->TryGetComponent<Dice>()))
			{
				dice->Toggle();
			}
			return false;
		});
}

void Dices::Roll()
{
	for (auto& diceHandle : m_dices)
	{
		auto& dice = *diceHandle;
		if (!dice.selected)
		{
			auto value = static_cast<uint8_t>(m_rollDist(m_gen));
			dice.value = value + 1;
			dice.sprite = m_sprites[value];
			dice.Roll(m_shakeDist(m_gen));
		}
	}
}

void Dices::Destroy()
{
	EventHandler::Remove(m_mouseDownHandle);
	m_rollButton->mouseClick.Remove(m_rollHandle);
}