#pragma once
#include "EntrypointHelpers.hpp"

extern constexpr void Initialize(Init init);
extern constexpr void RegisterResources(Resources resources);
extern constexpr void RegisterComponents(Components components);
extern constexpr void CreateRenderPipeline(TwoD::RenderPipeline& pipeline);

int main(void)
{
	TwoD::App::InitInfo info;
	TwoD::RenderPipeline pipeline;
	TwoD::App app;

	Initialize({info});
	RegisterResources({app});
	RegisterComponents({app});

	info.basePaths.push_back(".");

	app.Init(info);
	CreateRenderPipeline(pipeline);
	TwoD::App::Get<TwoD::RenderSystem>().SetPipeline(pipeline);
	app.Run();
}