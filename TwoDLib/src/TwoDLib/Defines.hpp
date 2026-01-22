#include "yaml-cpp/yaml.h"

namespace TwoD
{
	enum class RenderLocation
	{
		InWorld,
		OnScreen
	};
}

namespace YAML
{
	template<>
	struct convert<TwoD::RenderLocation>
	{
		static Node encode(const TwoD::RenderLocation& rhs)
		{
			switch (rhs)
			{
			case TwoD::RenderLocation::InWorld:
				return Node("inWorld");
			case TwoD::RenderLocation::OnScreen:
				return Node("onScreen");
			}
		}
		static bool decode(const Node& node, TwoD::RenderLocation& rhs)
		{
			const auto& str = node.as<std::string>();
			if (str == "inWorld")
			{
				rhs = TwoD::RenderLocation::InWorld;
				return true;
			}
			if (str == "onScreen")
			{
				rhs = TwoD::RenderLocation::OnScreen;
				return true;
			}
			return false;
		}
	};
}