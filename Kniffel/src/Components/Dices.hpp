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
	std::uniform_int_distribution<int> m_dist{ 0, 5 };

	TwoD::ComponentHandle<TwoD::Button> m_rollButton;
	TwoD::Event<>::Handle m_rollHandle;
};