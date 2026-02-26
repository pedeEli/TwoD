#pragma once
#include "TwoD.hpp"

TD_YAML_ENUM_WITH_BASE(TwoD, Anchor, uint8_t,
	TD_YAML_ENUM_FIELD(CENTER, 0),
	TD_YAML_ENUM_FIELD(TOP, 1),
	TD_YAML_ENUM_FIELD(RIGHT, 2),
	TD_YAML_ENUM_FIELD(BOTTOM, 4),
	TD_YAML_ENUM_FIELD(LEFT, 8),
	TD_YAML_ENUM_FIELD(TOP_RIGHT, TOP | RIGHT),
	TD_YAML_ENUM_FIELD(TOP_LEFT, TOP | LEFT),
	TD_YAML_ENUM_FIELD(BOTTOM_RIGHT, BOTTOM | RIGHT),
	TD_YAML_ENUM_FIELD(BOTTOM_LEFT, BOTTOM | LEFT)
)

namespace TwoD
{
	class RectTransform : public Component
	{
		using Component::Component;
	public:
		void SetSize(glm::fvec2 size);
		void SetWidth(float width);
		void SetHeight(float height);
		glm::fvec2 GetSize() const;
		float GetWidth() const;
		float GetHeight() const;

		void SetOffset(glm::fvec2 offset);
		glm::fvec2 GetOffset() const;

		void SetAnchor(Anchor anchor);

		void UpdateTransform();
		void UpdateTransform(glm::fvec2 parentSize);

	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD_WITH_UPDATER(glm::fvec2, size, {}, SetSize(size);),
			TD_COMPONENT_FIELD_WITH_UPDATER(glm::fvec2, offset, {}, SetOffset(size);),
			TD_COMPONENT_FIELD_WITH_UPDATER(Anchor, anchor, Anchor::CENTER, SetAnchor(anchor);)
		)
	};
}