#include "tdpch.hpp"
#include "LayoutSize.hpp"

namespace TwoD
{
	void LayoutSize::Start()
	{
		m_transform = GetComponent<UITransform>();
	}

	void LayoutSize::Update([[maybe_unused]] float delta)
	{
		auto& transform = *m_transform;
		transform.size = size;
		if (widthMode == SizingMode::PERCENTAGE || heightMode == SizingMode::PERCENTAGE)
		{
			auto& parent = transform.GetParent()->GetComponent<UITransform>();
			if (widthMode == SizingMode::PERCENTAGE)
			{
				transform.size.x = parent.size.x * size.x * 0.01f;
			}
			if (heightMode == SizingMode::PERCENTAGE)
			{
				transform.size.y = parent.size.y * size.y * 0.01f;
			}
		}
		if (widthMode == SizingMode::FIT_CONTENT)
		{
			transform.size.x = minSize.x;
		}
		if (heightMode == SizingMode::FIT_CONTENT)
		{
			transform.size.y = minSize.y;
		}
		transform.size = glm::clamp(transform.size, minSize, maxSize);
	}
}