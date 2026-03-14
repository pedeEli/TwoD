#include "tdpch.hpp"
#include "UITransform.hpp"

namespace TwoD
{
	void UITransform::UpdateMatrix(const std::vector<PropagationCallback>& callbacks, glm::fmat3x3* parent)
	{
		UpdateMatrix(
			callbacks,
			parent,
			m_parent ? m_parent->GetComponent<UITransform>().size : (size * 2.0f)
		);
	}
	void UITransform::UpdateMatrix(const std::vector<PropagationCallback>& callbacks, glm::fmat3x3* parent, glm::fvec2 parentSize)
	{
		CalcMatrix(parent);

		position = { 0.0f, 0.0f };
		if ((anchor & Anchor::TOP) != Anchor::CENTER)
		{
			position.y = (size.y - parentSize.y) * 0.5f;
		}
		else if ((anchor & Anchor::BOTTOM) != Anchor::CENTER)
		{
			position.y = (parentSize.y - size.y) * 0.5f;
		}
		if ((anchor & Anchor::LEFT) != Anchor::CENTER)
		{
			position.x = (size.x - parentSize.x) * 0.5f;
		}
		else if ((anchor & Anchor::RIGHT) != Anchor::CENTER)
		{
			position.x = (parentSize.x - size.x) * 0.5f;
		}
		position += offset;
		
		for (auto& callback : callbacks)
		{
			callback(*this);
		}

		for (auto child : m_children)
		{
			child->GetComponent<UITransform>().UpdateMatrix(callbacks, &m_worldMatrix, size);
		}
	}

	Rect<float> UITransform::GetRect() const
	{
		glm::fvec2 center = m_worldMatrix[2];
		return {
			center - size * 0.5f,
			center + size * 0.5f
		};
	}
}