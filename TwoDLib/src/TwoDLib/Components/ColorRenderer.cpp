#include "tdpch.hpp"
#include "ColorRenderer.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void ColorRenderer::StartBefore()
	{
		App::Get<RenderSystem>().SetDirty<ColorRenderer>();
	}

	void ColorRenderer::Destroy()
	{
		App::Get<RenderSystem>().SetDirty<ColorRenderer>();
	}
}