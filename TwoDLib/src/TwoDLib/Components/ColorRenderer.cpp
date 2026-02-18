#include "tdpch.hpp"
#include "ColorRenderer.hpp"


namespace TwoD
{
	void ColorRenderer::StartBefore()
	{
		App::Get<RenderSystem>().UpdateLayerFor<ColorRenderer>(layer);
	}

	void ColorRenderer::Destroy()
	{
		App::Get<RenderSystem>().UpdateLayerFor<ColorRenderer>(layer);
	}
}