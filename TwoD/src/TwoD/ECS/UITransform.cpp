#include "tdpch.hpp"
#include "UITransform.hpp"

namespace TwoD
{
	void UITransform::CreateLoadData(internal_load_data* loadData, const Deserializer& deserializer)
	{
		Transform::CreateLoadData(loadData, deserializer);
		glm::fvec2 m_size = { 0.0f, 0.0f };
		glm::fvec2 m_offset = { 0.0f, 0.0f };
		Anchor m_anchor = Anchor::CENTER;
		if (deserializer["size"])
		{
			if (!deserializer["size"].As<glm::fvec2>(loadData->size))
			{
				TD_CORE_ASSERT(false, "failed to load ui transform");
			}
		}
		if (deserializer["offset"])
		{
			if (!deserializer["offset"].As<glm::fvec2>(loadData->offset))
			{
				TD_CORE_ASSERT(false, "failed to load ui transform");
			}
		}
		if (deserializer["anchor"])
		{
			if (!deserializer["anchor"].As<Anchor>(loadData->anchor))
			{
				TD_CORE_ASSERT(false, "failed to load ui transform");
			}
		}
	}
	void UITransform::Load(const void* data)
	{
		auto* loadData = static_cast<const internal_load_data*>(data);
		m_size = loadData->size;
		m_offset = loadData->offset;
		m_anchor = loadData->anchor;
		Transform::Load(data);
	}

#ifdef TD_CREATE_DEBUGGER
	void UITransform::Debug()
	{
		Transform::Debug();
		bool changed = false;
		changed |= Debuggable<glm::fvec2>::Draw(m_size, "size");
		changed |= Debuggable<glm::fvec2>::Draw(m_offset, "offset");
		changed |= Debuggable<Anchor>::Draw(m_anchor, "anchor");
	}
#endif

	void UITransform::UpdateTransform()
	{
		if (m_parent)
		{
			UpdateTransform(m_parent->GetComponent<UITransform>().GetSize());
		}
		else
		{
			UpdateTransform(m_size);
		}
	}
	void UITransform::UpdateTransform(glm::fvec2 parentSize)
	{
		glm::fvec2 pos = { 0.0f, 0.0f };

		if ((m_anchor & Anchor::TOP) != Anchor::CENTER)
		{
			pos.y = (m_size.y - parentSize.y) * 0.5f;
		}
		else if ((m_anchor & Anchor::BOTTOM) != Anchor::CENTER)
		{
			pos.y = (parentSize.y - m_size.y) * 0.5f;
		}

		if ((m_anchor & Anchor::LEFT) != Anchor::CENTER)
		{
			pos.x = (m_size.x - parentSize.x) * 0.5f;
		}
		else if ((m_anchor & Anchor::RIGHT) != Anchor::CENTER)
		{
			pos.y = (parentSize.x - m_size.x) * 0.5f;
		}

		SetPosition(pos + m_offset);
		for (auto child : m_children)
		{
			child->GetComponent<UITransform>().UpdateTransform(m_size);
		}
	}


	void UITransform::SetSize(glm::fvec2 size)
	{
		m_size = size;
		UpdateTransform();
	}
	void UITransform::SetWidth(float width)
	{
		m_size.x = width;
		UpdateTransform();
	}
	void UITransform::SetHeight(float height)
	{
		m_size.y = height;
		UpdateTransform();
	}
	glm::fvec2 UITransform::GetSize() const
	{
		return m_size;
	}
	float UITransform::GetWidth() const
	{
		return m_size.x;
	}
	float UITransform::GetHeight() const
	{
		return m_size.y;
	}

	void UITransform::SetOffset(glm::fvec2 offset)
	{
		m_offset = offset;
		UpdateTransform();
	}
	glm::fvec2 UITransform::GetOffset() const
	{
		return m_offset;
	}

	void UITransform::SetAnchor(Anchor anchor)
	{
		m_anchor = anchor;
		UpdateTransform();
	}

	Rect<float> UITransform::GetRect() const
	{
		glm::fvec2 center = m_worldMatrix[2];
		return {
			center - m_size * 0.5f,
			center + m_size * 0.5f
		};
	}
}