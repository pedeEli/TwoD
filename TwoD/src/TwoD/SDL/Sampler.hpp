#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
    class Sampler
	{
    public:
        Sampler();
        Sampler(const Window* window, const SamplerInfo& info);
        ~Sampler();
        Sampler(const Sampler& other) = delete;
        Sampler(Sampler&& other) noexcept;
        Sampler& operator=(const Sampler& other) = delete;
        Sampler& operator=(Sampler&& other) noexcept;

    private:
        struct Raw;
        std::unique_ptr<Raw> m_raw;

        friend class RenderPass;
	};
}


