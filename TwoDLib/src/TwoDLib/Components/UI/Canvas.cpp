#include "tdpch.hpp"
#include "Canvas.hpp"

#include "RectTransform.hpp"

namespace TwoD
{
	void Canvas::Start()
	{
		ComponentHandle rectTransform = GetComponent<RectTransform>();
		auto& rt = *rectTransform;
		TD_CORE_ASSERT(rt.anchor == Anchor::CENTER);
		EventHandler::On<WindowResizedEvent>([rectTransform](auto& event)
			{
				rectTransform->SetSize({ event.x, event.y });
				return false;
			});
		int width, height;
		App::Get<Window>().GetSize(width, height);
		rt.SetSize({ width, height });
		GetComponent<Transform>().SetPosition(width * 0.5f, height * 0.5f);
	}
}