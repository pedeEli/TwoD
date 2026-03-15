#include "tdpch.hpp"
#include "Hitbox.hpp"

namespace TwoD
{
	bool Hitbox::IsInside(const glm::fvec2& pos) const
	{
		auto localPos = GetTransform()->GetInverseWorldMatrix() * glm::fvec3(pos, 1.0);
		return localPos.x >= x && localPos.x <= x + w && localPos.y >= y && localPos.y <= y + h;
	}

	const Hitbox* Hitbox::Hit(const glm::fvec2& pos)
	{
		auto& hitboxs = ECS::GetComponents<Hitbox>();

		int32_t currentLayer = std::numeric_limits<int32_t>::min();
		const Hitbox* currentHitbox = nullptr;

		for (auto& hitbox : hitboxs)
		{
			if (hitbox.IsInside(pos))
			{
				if (hitbox.layer > currentLayer)
				{
					currentLayer = hitbox.layer;
					currentHitbox = &hitbox;
				}
			}
		}

		return currentHitbox;
	}
}