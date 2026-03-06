#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Assets/Sprite.hpp"

namespace TwoD
{
	class SpriteRenderer : public Component
	{
		using Component::Component;
	public:
		void StartBefore() override;
		void Destroy() override;

		void SetLayer(uint32_t layer);

	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD_WITH_UPDATER(int32_t, layer, 0, SetLayer(layer);),
			TD_COMPONENT_FIELD(Sprite*, sprite),
			TD_COMPONENT_FIELD(std::optional<uint32_t>, slice, {})
		)
	};
}