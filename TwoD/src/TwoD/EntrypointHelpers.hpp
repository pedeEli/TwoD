#pragma once
#include "TwoD.hpp"

class RenderLayers
{
public:
	constexpr RenderLayers(TwoD::App& app) : m_app(app) {}

	template<class Layer>
	requires(std::is_base_of_v<TwoD::RenderLayer, Layer>)
	constexpr void Register()
	{
		m_app.RegisterRenderLayer<Layer>();
	}

	template<class Layer>
	requires(std::is_base_of_v<TwoD::RenderLayer2, Layer>)
	constexpr void Register2()
	{
		m_app.RegisterRenderLayer2<Layer>();
	}

private:
	TwoD::App& m_app;
};

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