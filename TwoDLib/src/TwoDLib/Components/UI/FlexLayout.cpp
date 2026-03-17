#include "tdpch.hpp"
#include "FlexLayout.hpp"
#include "LayoutSize.hpp"

namespace TwoD
{
	void FlexLayout::ApplyFlexLayout(Transform& transform)
	{
		if (auto* flexLayout = transform.TryGetComponent<FlexLayout>())
		{
			flexLayout->Apply(transform.GetComponent<UITransform>());
		}
	}

	void FlexLayout::Apply(UITransform& uiTransform) const
	{
		const bool isRow = direction == FlexDirection::ROW;
		const glm::fvec2 innerSize = uiTransform.size - padding * 2.0f;
		auto& children = uiTransform.GetChildren();

		struct Item
		{
			UITransform* transform;
			float mainSize;
			float crossSize;
		};

		std::vector<Item> items;
		items.reserve(children.size());

		float totalMainSize = 0.0f;
		float maxCrossSize = 0.0f;
		for (auto child : children)
		{
			auto* transform = &child->GetComponent<UITransform>();
			float mainSize = isRow ? transform->size.x : transform->size.y;
			float crossSize = isRow ? transform->size.y : transform->size.x;
			totalMainSize += mainSize;
			maxCrossSize = std::max(maxCrossSize, crossSize);
			items.emplace_back(transform, mainSize, crossSize);
		}

		if (items.empty())
		{
			return;
		}

		const float n = static_cast<float>(items.size());
		const float totalGaps = gap * (n - 1.0f);
		const float mainLength = isRow ? innerSize.x : innerSize.y;
		const float crossLength = isRow ? innerSize.y : innerSize.x;
		float freeSpace = mainLength - totalMainSize - totalGaps;

		if (auto* layoutSize = TryGetComponent<LayoutSize>())
		{
			layoutSize->minSize = isRow ? glm::fvec2(totalMainSize, maxCrossSize) : glm::fvec2(maxCrossSize, totalMainSize);
			layoutSize->minSize += padding * 2.0f;
		}

		float cursor = 0.0f;
		float extraGap = 0.0f;

		switch (justify)
		{
		case JustifyContent::START:
			cursor = -mainLength * 0.5f;
			break;
		case JustifyContent::END:
			cursor = -mainLength * 0.5f + freeSpace;
			break;
		case JustifyContent::CENTER:
			cursor = -mainLength * 0.5f + freeSpace * 0.5f;
			break;
		case JustifyContent::SPACE_BETWEEN:
			extraGap = n > 1.0f ? freeSpace / (n - 1.0f) : 0.0f;
			cursor = -mainLength * 0.5f;
			break;
		case JustifyContent::SPACE_AROUND:
			extraGap = freeSpace / n;
			cursor = -mainLength * 0.5f + extraGap * 0.5f;
			break;
		}

		for (auto& item : items)
		{
			float crossOffset = 0.0f;
			switch (align)
			{
			case AlignItems::START:
				crossOffset = -crossLength * 0.5f + item.crossSize * 0.5f;
				break;
			case AlignItems::END:
				crossOffset = -crossLength * 0.5f - item.crossSize * 0.5f;
				break;
			}

			float mainPosition = cursor + item.mainSize * 0.5f;
			glm::fvec2 position = isRow ? glm::fvec2(mainPosition, crossOffset) : glm::fvec2(crossOffset, -mainPosition);

			item.transform->offset = position + padding;
			item.transform->anchor = Anchor::CENTER;

			cursor += item.mainSize + gap + extraGap;
		}
	}
}