#pragma once
#include "TwoD.hpp"

class Components
{
public:
	constexpr Components(TwoD::App& app) : m_app(app) {}

	template<class T>
	requires(std::is_base_of_v<TwoD::Component, T>)
	constexpr void Register()
	{
		m_app.RegisterComponent<T>();
	}

private:
	TwoD::App& m_app;
};

class Resources
{
public:
	constexpr Resources(TwoD::App& app) : m_app(app) {}

	template<class T>
	requires(std::is_base_of_v<TwoD::Asset, T>)
	constexpr void Register()
	{
		m_app.RegisterResource<T>();
	}

private:
	TwoD::App& m_app;
};

class Init
{
public:
	constexpr Init(TwoD::App::InitInfo& info) : m_info(info) {}

	constexpr void SetSize(int width, int height)
	{
		m_info.width = width;
		m_info.height = height;
	}
	constexpr void SetTitle(std::string_view title)
	{
		m_info.title = title;
	}
	constexpr void SetWindowFlag(TwoD::WindowFlags flag)
	{
		m_info.windowFlags = m_info.windowFlags ^ flag;
	}
	constexpr void SetStartScene(std::string_view scene)
	{
		m_info.startScene = scene;
	}
	constexpr void AddBasePath(const std::filesystem::path& path)
	{
		m_info.basePaths.push_back(path);
	}


private:
	TwoD::App::InitInfo& m_info;
};

namespace TwoDLib
{
	constexpr static void Initialize(Init& init)
	{
		//info.AddBasePath(".")
	}

	constexpr static void RegisterResources(Resources& resources)
	{
		resources.Register<TwoD::Sprite>();
		resources.Register<TwoD::Font>();
		resources.Register<TwoD::SpriteAtlas>();
	}

	constexpr static void RegisterComponents(Components& components)
	{
		components.Register<TwoD::Camera>();
		components.Register<TwoD::Hitbox>();
		components.Register<TwoD::SpriteRenderer>();
		components.Register<TwoD::TextRenderer>();
		components.Register<TwoD::Transform>();
	}
}

extern constexpr void Initialize(Init init);
extern constexpr void RegisterResources(Resources resources);
extern constexpr void RegisterComponents(Components components);

int main(void)
{
	TwoD::App::InitInfo info;
	TwoD::App app;

	Initialize({info});
	RegisterResources({app});
	RegisterComponents({app});

	info.basePaths.push_back(".");

	app.Init(info);
	app.Run();
}