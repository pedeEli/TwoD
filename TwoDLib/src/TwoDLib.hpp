#pragma once
#include "TwoD/EntrypointHelpers.hpp"

#include "TwoDLib/Assets/Font.hpp"
#include "TwoDLib/Assets/Sprite.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"
#include "TwoDLib/Assets/Audio.hpp"
#include "TwoDLib/Assets/Mesh.hpp"

#include "TwoDLib/Components/Camera.hpp"
#include "TwoDLib/Components/Hitbox.hpp"
#include "TwoDLib/Components/SpriteRenderer.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"
#include "TwoDLib/Components/ScissorRect.hpp"
#include "TwoDLib/Components/MeshRenderer.hpp"

#include "TwoDLib/Components/UI/Button.hpp"
#include "TwoDLib/Components/UI/Text.hpp"
#include "TwoDLib/Components/UI/FlexLayout.hpp"

#include "TwoDLib/RenderHandlers/SpriteRenderHandler.hpp"
#include "TwoDLib/RenderHandlers/ColorRenderHandler.hpp"
#include "TwoDLib/RenderHandlers/TextRenderHandler.hpp"
#include "TwoDLib/RenderHandlers/MeshRenderHandler.hpp"

#include "TwoDLib/Event.hpp"

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
		resources.Register<TwoD::Mesh>();
	}

	constexpr static void RegisterComponents(Components& components)
	{
		components.Register<TwoD::Camera>();
		components.Register<TwoD::Hitbox>();
		components.Register<TwoD::SpriteRenderer>();
		components.Register<TwoD::TextRenderer>();
		components.Register<TwoD::ColorRenderer>();
		components.Register<TwoD::ScissorRect>();
		components.Register<TwoD::ComputedScissorRect>();
		components.Register<TwoD::MeshRenderer>();

		components.Register<TwoD::Button>();
		components.Register<TwoD::Text>();
		components.Register<TwoD::FlexLayout>();
	}

	constexpr static void RegisterRenderHandlers(RenderHandlers& renderHandlers)
	{
		renderHandlers.Register<TwoD::SpriteRenderHandler>();
		renderHandlers.Register<TwoD::ColorRenderHandler>();
		renderHandlers.Register<TwoD::TextRenderHandler>();
		renderHandlers.Register<TwoD::MeshRenderHandler>();
	}

	void Misc();
}