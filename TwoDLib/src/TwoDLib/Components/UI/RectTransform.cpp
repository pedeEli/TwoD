#include "tdpch.hpp"
#include "RectTransform.hpp"

namespace TwoD
{
	void RectTransform::StartBefore()
	{
		transform = GetComponent<Transform>();
	}

	void RectTransform::UpdateTransform()
	{
		auto parent = transform->GetParent();
		if (parent)
		{
			UpdateTransform(parent->GetComponent<RectTransform>().GetSize());
		}
		else
		{
			UpdateTransform(size);
		}
	}
	void RectTransform::UpdateTransform(glm::fvec2 parentSize)
	{
		auto& t = *transform;

		glm::fvec2 pos = { 0.0f, 0.0f };

		if ((anchor & Anchor::TOP) != Anchor::CENTER)
		{
			pos.y = (size.y - parentSize.y) * 0.5f;
		}
		else if ((anchor & Anchor::BOTTOM) != Anchor::CENTER)
		{
			pos.y = (parentSize.y - size.y) * 0.5f;
		}

		if ((anchor & Anchor::LEFT) != Anchor::CENTER)
		{
			pos.x = (size.x - parentSize.x) * 0.5f;
		}
		else if ((anchor & Anchor::RIGHT) != Anchor::CENTER)
		{
			pos.y = (parentSize.x - size.x) * 0.5f;
		}

		t.SetPosition(pos + offset);
		
		auto& children = t.GetChildren();
		for (auto child : children)
		{
			child->GetComponent<RectTransform>().UpdateTransform(size);
		}
	}

	void RectTransform::SetSize(glm::fvec2 size)
	{
		this->size = size;
		UpdateTransform();
	}
	void RectTransform::SetWidth(float width)
	{
		size.x = width;
		UpdateTransform();
	}
	void RectTransform::SetHeight(float height)
	{
		size.y = height;
		UpdateTransform();
	}
	glm::fvec2 RectTransform::GetSize() const
	{
		return size;
	}
	float RectTransform::GetWidth() const
	{
		return size.x;
	}
	float RectTransform::GetHeight() const
	{
		return size.y;
	}

	void RectTransform::SetOffset(glm::fvec2 offset)
	{
		this->offset = offset;
		UpdateTransform();
	}
	glm::fvec2 RectTransform::GetOffset() const
	{
		return offset;
	}

	void RectTransform::SetAnchor(Anchor anchor)
	{
		this->anchor = anchor;
		UpdateTransform();
	}
}