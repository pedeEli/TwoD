#include "tdpch.hpp"
#include "SpriteRenderer.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void SpriteRenderer::StartBefore()
	{
		App::Get<RenderSystem>().SetDirty<SpriteRenderer>();
	}

	void SpriteRenderer::Destroy()
	{
		App::Get<RenderSystem>().SetDirty<SpriteRenderer>();
	}
}