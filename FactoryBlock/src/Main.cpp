#include "TwoD/Entrypoint.hpp"
#include "TwoDLib.hpp"

#include "Components/Player.hpp"
#include "Components/Ground.hpp"
#include "Components/Vein.hpp"
#include "Components/Ore.hpp"
#include "Components/Tile.hpp"

constexpr static void Initialize(Init init)
{
	TwoDLib::Initialize(init);
	init.SetSize(800, 600);
	init.SetTitle("Test game");
	init.SetStartScene("test");
	init.SetWindowFlag(TwoD::WindowFlags::RESIZABLE);
	init.SetWindowFlag(TwoD::WindowFlags::HIGH_PIXEL_DENSITY);
	init.SetWindowFlag(TwoD::WindowFlags::MAXIMIZED);
}

constexpr static void RegisterResources(Resources resources)
{
	TwoDLib::RegisterResources(resources);
}

constexpr static void RegisterComponents(Components components)
{
	TwoDLib::RegisterComponents(components);
	components.Register<Player>();
	components.Register<Vein>();
	components.Register<Ore>();
	components.Register<Ground>();
	components.Register<Tile>();
}

constexpr static void RegisterRenderHandlers(RenderHandlers renderHandlers)
{
	TwoDLib::RegisterRenderHandlers(renderHandlers);
}

void Misc()
{
	TwoDLib::Misc();
}