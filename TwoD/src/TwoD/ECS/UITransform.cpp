#include "tdpch.hpp"
#include "UITransform.hpp"

namespace TwoD
{
	void UITransform::CreateLoadData(internal_load_data* loadData, const YAML::Node& node)
	{
		Transform::CreateLoadData(loadData, node);
		glm::fvec2 m_size = { 0.0f, 0.0f };
		glm::fvec2 m_offset = { 0.0f, 0.0f };
		Anchor m_anchor = Anchor::CENTER;
		if (node["size"])
		{
			loadData->size = node["size"].as<glm::fvec2>();
		}
		if (node["offset"])
		{
			loadData->offset = node["offset"].as<glm::fvec2>();
		}
		if (node["anchor"])
		{
			loadData->anchor = node["anchor"].as<Anchor>();
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

#ifdef TD_IMGUI
	void UITransform::Debug()
	{
		Transform::Debug();
		bool changed = false;
		changed |= ImGuiType<glm::fvec2>::Draw(m_size, "size");
		changed |= ImGuiType<glm::fvec2>::Draw(m_offset, "offset");
		changed |= ImGuiType<Anchor>::Draw(m_anchor, "anchor");
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
}