#pragma once
#include "TwoD.hpp"
#include "TwoDLib.hpp"

TD_COMPONENT(
	(TD_NAME(Dice)),
	(
		TD_COMPONENT_FIELD(float, speed, TD_INIT(20.0f)),
		TD_COMPONENT_FIELD(glm::fvec2, targetPosition, TD_INIT({})),
		TD_COMPONENT_FIELD(glm::fvec4, targetColor, TD_INIT({ 255.0f, 255.0f, 255.0f, 255.0f })),
		TD_COMPONENT_FIELD(TwoD::Sprite*, sprite, TD_INIT(nullptr)),
		TD_COMPONENT_FIELD(uint8_t, index, TD_INIT(0)),
		TD_COMPONENT_FIELD(bool, selected, TD_INIT(false)),
		TD_COMPONENT_FIELD(float, shakeDuration, TD_INIT(0.3f)),
		TD_COMPONENT_FIELD(float, shakeMagnitude, TD_INIT(0.045f))
	)
)
public:
	void Start() override;
	void Update(float delta) override;

	void Toggle();
	void Roll(float shakeOffset);

public:
	uint8_t value = 0;

private:
	TwoD::ComponentHandle<TwoD::SpriteRenderer> m_renderer;
	float m_shakeTimer = 0.0f;
	float m_shakeOffset = 0.0f;
};