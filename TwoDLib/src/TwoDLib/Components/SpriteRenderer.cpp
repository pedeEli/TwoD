#include "tdpch.hpp"
#include "SpriteRenderer.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void SpriteRenderer::StartBefore()
	{
		App::Get<RenderSystem>().UpdateLayerFor<SpriteRenderer>(layer);
	}

	void SpriteRenderer::Destroy()
	{
		App::Get<RenderSystem>().UpdateLayerFor<SpriteRenderer>(layer);
	}
}