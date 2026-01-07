#include "TwoD.hpp"

#include "Components/Player.hpp"
#include "Components/Ground.hpp"
#include "Components/Vein.hpp"
#include "Components/Ore.hpp"

constexpr TwoD::WindowFlags windowFlags = TwoD::WindowFlags::RESIZABLE | TwoD::WindowFlags::HIGH_PIXEL_DENSITY | TwoD::WindowFlags::MAXIMIZED;


int main()
{
	TwoD::AppInfo info{
		.width = 800,
		.height = 600,
		.title = "Test game",
		.windowFlags = windowFlags,
		.startScene = "test",
#ifdef BASE_PATH
		.basePath = BASE_PATH
#endif
	};

	TwoD::App app;

	app.RegisterResource<TwoD::Sprite>();
	app.RegisterResource<TwoD::Font>();
	app.RegisterResource<TwoD::SpriteAtlas>();
	//app.RegisterResource<TwoD::Mesh>();
	//app.RegisterResource<TwoD::Prefab>();

	app.RegisterComponent<TwoD::Camera>();
	app.RegisterComponent<TwoD::Hitbox>();
	app.RegisterComponent<TwoD::SpriteRenderer>();
	app.RegisterComponent<TwoD::TextRenderer>();
	app.RegisterComponent<TwoD::Transform>();
	app.RegisterComponent<Player>();
	app.RegisterComponent<Vein>();
	app.RegisterComponent<Ore>();
	app.RegisterComponent<Ground>();

	app.Init(info);
	app.Run();
}