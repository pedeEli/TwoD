#pragma once
#include "TwoD/EntrypointHelpers.hpp"

#include "TwoDLib/Assets/Font.hpp"
#include "TwoDLib/Assets/Sprite.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"
#include "TwoDLib/Assets/Audio.hpp"

#include "TwoDLib/Components/Camera.hpp"
#include "TwoDLib/Components/Hitbox.hpp"
#include "TwoDLib/Components/SpriteRenderer.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"

#include "TwoDLib/Components/UI/RectTransform.hpp"
#include "TwoDLib/Components/UI/Canvas.hpp"
#include "TwoDLib/Components/UI/Button.hpp"

#include "TwoDLib/RenderHandlers/SpriteRenderHandler.hpp"
#include "TwoDLib/RenderHandlers/ColorRenderHandler.hpp"
#include "TwoDLib/RenderHandlers/TextRenderHandler.hpp"

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
		resources.Register<TwoD::Audio>();
	}

	constexpr static void RegisterComponents(Components& components)
	{
		components.Register<TwoD::Camera>();
		components.Register<TwoD::Hitbox>();
		components.Register<TwoD::SpriteRenderer>();
		components.Register<TwoD::TextRenderer>();
		components.Register<TwoD::ColorRenderer>();

		components.Register<TwoD::RectTransform>();
		components.Register<TwoD::Canvas>();
		components.Register<TwoD::Button>();
	}

	constexpr static void RegisterRenderHandlers(RenderHandlers& renderHandlers)
	{
		renderHandlers.Register<TwoD::SpriteRenderHandler>();
		renderHandlers.Register<TwoD::ColorRenderHandler>();
		renderHandlers.Register<TwoD::TextRenderHandler>();
	}
}