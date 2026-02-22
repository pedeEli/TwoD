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

	void SpriteRenderer::SetLayer(uint32_t layer)
	{
		this->layer = layer;
		App::Get<RenderSystem>().UpdateLayerFor<SpriteRenderer>(layer);
	}
}