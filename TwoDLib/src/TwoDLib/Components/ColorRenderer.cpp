#include "tdpch.hpp"
#include "ColorRenderer.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void ColorRenderer::StartBefore()
	{
		App::Get<RenderSystem>().SetDirty<ColorRenderer>();
		App::Get<RenderSystem2>().UpdateLayerFor<ColorRenderer>(layer);
	}

	void ColorRenderer::Destroy()
	{
		App::Get<RenderSystem>().SetDirty<ColorRenderer>();
		App::Get<RenderSystem2>().UpdateLayerFor<ColorRenderer>(layer);
	}
}