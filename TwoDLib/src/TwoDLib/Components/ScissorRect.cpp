#include "tdpch.hpp"
#include "ScissorRect.hpp"

namespace TwoD
{
	void ScissorRect::ComputeRect(Transform& transform)
	{
		auto* rect = transform.TryGetComponent<ScissorRect>();
		ComputedScissorRect* parentRect = nullptr;
		auto parent = transform.GetParent();
		bool hasParent = parent && (parentRect = parent->TryGetComponent<ComputedScissorRect>());

		if (!rect && !hasParent)
		{
			return;
		}

		auto* computed = transform.TryGetComponent<ComputedScissorRect>();
		if (!computed)
		{
			computed = &transform.AddComponent<ComputedScissorRect>();
		}

		if (hasParent)
		{
			computed->rect = parentRect->rect;
		}
		else
		{
			computed->rect = {
				transform.GetWorldMatrix() * glm::fvec3(rect->rect.min, 1.0f),
				transform.GetWorldMatrix() * glm::fvec3(rect->rect.max, 1.0f),
			};
		}
	}
}