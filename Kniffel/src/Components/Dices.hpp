#pragma once
#include <random>
#include "TwoD.hpp"
#include "TwoDLib.hpp"
#include "Dice.hpp"

TD_COMPONENT(
	(TD_NAME(Dices)),
	()
)
public:
	void Start() override;
	void Destroy() override;

private:
	void Roll();

private:
	TwoD::EventHandler::Handle m_mouseDownHandle;

	std::array<TwoD::Sprite*, 6> m_sprites = {};
	std::array<TwoD::ComponentHandle<Dice>, 6> m_dices = {};

	std::mt19937 m_gen;
	std::uniform_int_distribution<int> m_rollDist{ 0, 5 };
	std::uniform_real_distribution<float> m_shakeDist{ -1.0f, 1.0f };

	TwoD::ComponentHandle<TwoD::Button> m_rollButton;
	TwoD::Event<>::Handle m_rollHandle;
};