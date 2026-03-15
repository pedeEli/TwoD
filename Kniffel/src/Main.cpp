#include "TwoD/Entrypoint.hpp"
#include "TwoDLib.hpp"

#include "Components/Dices.hpp"
#include "Components/Dice.hpp"

constexpr void Initialize(Init init)
{
	TwoDLib::Initialize(init);
	init.SetSize(800, 600);
	init.SetTitle("Kniffel");
	init.SetStartScene("game");
	init.SetWindowFlag(TwoD::WindowFlags::RESIZABLE);
	init.SetWindowFlag(TwoD::WindowFlags::HIGH_PIXEL_DENSITY);
	init.SetWindowFlag(TwoD::WindowFlags::MAXIMIZED);
}

constexpr void RegisterResources(Resources resources)
{
	TwoDLib::RegisterResources(resources);
}

constexpr void RegisterComponents(Components components)
{
	TwoDLib::RegisterComponents(components);
	components.Register<Dices>();
	components.Register<Dice>();
}

constexpr void RegisterRenderHandlers(RenderHandlers renderHandlers)
{
	TwoDLib::RegisterRenderHandlers(renderHandlers);
}

void Misc()
{
	TwoDLib::Misc();
}