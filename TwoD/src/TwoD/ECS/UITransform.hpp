#pragma once
#include "ECSDefines.hpp"
#include "Transform.hpp"
#include "TwoD/Math/Rect.hpp"

TD_ENUM(
	(TD_NAMESPACE(TwoD), TD_NAME(Anchor), TD_BASE(uint8_t)),
	(
		TD_ENUM_FIELD(CENTER, TD_INIT(0)),
		TD_ENUM_FIELD(TOP, TD_INIT(1)),
		TD_ENUM_FIELD(RIGHT, TD_INIT(2)),
		TD_ENUM_FIELD(BOTTOM, TD_INIT(4)),
		TD_ENUM_FIELD(LEFT, TD_INIT(8)),
		TD_ENUM_FIELD(TOP_RIGHT, TD_INIT(TOP | RIGHT), TD_NO_DEBUG),
		TD_ENUM_FIELD(TOP_LEFT, TD_INIT(TOP | LEFT), TD_NO_DEBUG),
		TD_ENUM_FIELD(BOTTOM_RIGHT, TD_INIT(BOTTOM | RIGHT), TD_NO_DEBUG),
		TD_ENUM_FIELD(BOTTOM_LEFT, TD_INIT(BOTTOM | LEFT), TD_NO_DEBUG)
	)
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
		static void CreateLoadData(internal_load_data* loadData, const Deserializer& deserializer);
		void Load(const void* data) override;
#ifdef TD_CREATE_DEBUGGER
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

