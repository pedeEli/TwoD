#pragma once
#include "EntrypointHelpers.hpp"

extern constexpr void Initialize(Init init);
extern constexpr void RegisterResources(Resources resources);
extern constexpr void RegisterComponents(Components components);
extern constexpr void RegisterRenderLayers(RenderLayers renderLayers);

int main(void)
{
	TwoD::App::InitInfo info;
	TwoD::App app;

	Initialize({info});
	RegisterResources({app});
	RegisterComponents({app});
	RegisterRenderLayers({app});

	info.basePaths.push_back(".");

	app.Init(info);
	app.Run();
}