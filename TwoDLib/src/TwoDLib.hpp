#pragma once
#include "TwoD/EntrypointHelpers.hpp"

#include "TwoDLib/Assets/Font.hpp"
#include "TwoDLib/Assets/Sprite.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"

#include "TwoDLib/Components/Camera.hpp"
#include "TwoDLib/Components/Hitbox.hpp"
#include "TwoDLib/Components/Canvas.hpp"
#include "TwoDLib/Components/SpriteRenderer.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"

#include "TwoDLib/RenderLayers/SpriteRenderLayer.hpp"
#include "TwoDLib/RenderLayers/TextRenderLayer.hpp"
#include "TwoDLib/RenderLayers/ColorRenderLayer.hpp"

#include "TwoDLib/RenderLayers2/SpriteRenderLayer2.hpp"
#include "TwoDLib/RenderLayers2/ColorRenderLayer2.hpp"
#include "TwoDLib/RenderLayers2/TextRenderLayer2.hpp"

namespace TwoDLib
{
	constexpr static void Initialize(Init& init)
	{
		init.AddBasePath("../TwoDLib/assets");
	}

	constexpr static void RegisterResources(Resources& resources)
	{
		resources.Register<TwoD::Sprite>();
		resources.Register<TwoD::Font>();
		resources.Register<TwoD::Shader>();
		resources.Register<TwoD::SpriteAtlas>();
	}

	constexpr static void RegisterComponents(Components& components)
	{
		components.Register<TwoD::Camera>();
		components.Register<TwoD::Hitbox>();
		components.Register<TwoD::Canvas>();
		components.Register<TwoD::SpriteRenderer>();
		components.Register<TwoD::TextRenderer>();
		components.Register<TwoD::ColorRenderer>();
	}

	constexpr static void RegisterRenderLayers(RenderLayers& renderLayers)
	{
		renderLayers.Register<TwoD::SpriteRenderLayer>();
		renderLayers.Register<TwoD::TextRenderLayer>();
		renderLayers.Register<TwoD::ColorRenderLayer>();

		renderLayers.Register2<TwoD::SpriteRenderLayer2>();
		renderLayers.Register2<TwoD::ColorRenderLayer2>();
		renderLayers.Register2<TwoD::TextRenderLayer2>();
	}
}