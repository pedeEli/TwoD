#include "tdpch.hpp"
#include "Hitbox.hpp"

namespace TwoD
{
	bool Hitbox::IsInside(const glm::fvec2& pos) const
	{
		auto localPos = GetComponent<Transform>().GetInverseWorldMatrix() * glm::fvec3(pos, 1.0);
		return localPos.x >= x && localPos.x <= x + w && localPos.y >= y && localPos.y <= y + h;
	}

	std::optional<const Hitbox*> Hitbox::Hit(const glm::fvec2& pos)
	{
		auto& ecs = App::Get<ECS>();
		auto& hitboxs = ecs.GetComponents<Hitbox>();

		int32_t currentLayer = std::numeric_limits<int32_t>::min();
		std::optional<const Hitbox*> currentHitbox = {};

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