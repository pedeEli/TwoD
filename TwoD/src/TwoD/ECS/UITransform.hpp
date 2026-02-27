#pragma once
#include "ECSDefines.hpp"
#include "Transform.hpp"
#include "TwoD/Core/YAML.hpp"
#include "TwoD/Math/Rect.hpp"

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
	class UITransform : public Transform
	{
		using Transform::Transform;
	public:
		struct internal_load_data : public Transform::internal_load_data
		{
			glm::fvec2 size = { 0.0f, 0.0f };
			glm::fvec2 offset = { 0.0f, 0.0f };
			Anchor anchor = Anchor::CENTER;
		};
		static void CreateLoadData(internal_load_data* loadData, const YAML::Node& node);
		void Load(const void* data) override;
#ifdef TD_IMGUI
		void Debug() override;
#endif

		void SetSize(glm::fvec2 size);
		void SetWidth(float width);
		void SetHeight(float height);
		glm::fvec2 GetSize() const;
		float GetWidth() const;
		float GetHeight() const;

		void SetOffset(glm::fvec2 offset);
		glm::fvec2 GetOffset() const;

		void SetAnchor(Anchor anchor);

		Rect<float> GetRect() const;

	private:
		void UpdateTransform();
		void UpdateTransform(glm::fvec2 parentSize);

	private:
		glm::fvec2 m_size = { 0.0f, 0.0f };
		glm::fvec2 m_offset = { 0.0f, 0.0f };
		Anchor m_anchor = Anchor::CENTER;
	};
}

