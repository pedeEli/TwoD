#include "tdpch.hpp"
#include "SpriteRenderer.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void SpriteRenderer::StartBefore()
	{
		App::Get<RenderSystem>().SetDirty<SpriteRenderer>();
		App::Get<RenderSystem2>().UpdateLayerFor<SpriteRenderer>(layer);
	}

	void SpriteRenderer::Destroy()
	{
		App::Get<RenderSystem>().SetDirty<SpriteRenderer>();
		App::Get<RenderSystem2>().UpdateLayerFor<SpriteRenderer>(layer);
	}
}