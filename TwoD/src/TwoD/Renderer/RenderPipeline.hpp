#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "RenderLayer.hpp"

namespace TwoD
{
	class RenderSystem;

	class RenderPipeline
	{
	public:
		RenderPipeline() = default;
		~RenderPipeline() = default;
		RenderPipeline(RenderPipeline& other) noexcept = delete;
		RenderPipeline(RenderPipeline&& other) noexcept = default;
		RenderPipeline& operator=(RenderPipeline& other) noexcept = delete;
		RenderPipeline& operator=(RenderPipeline&& other) noexcept = default;

		template<class Layer>
		requires(std::is_base_of_v<RenderLayer, Layer>)
		void AddLayer()
		{
			auto layer = std::make_unique<Layer>();
			const auto& types = layer->GetRendererTypes();
			auto index = m_layers.size();
			m_layers.push_back(std::move(layer));

			for (const auto& type : types)
			{
				auto it = m_typesToLayers.find(type);
				if (it == m_typesToLayers.end())
				{
					m_typesToLayers[type] = { index };
					continue;
				}
				it->second.push_back(index);
			}
		}
		
		template<class Layer>
		requires(std::is_base_of_v<RenderLayer, Layer>)
		Layer& GetLayer()
		{
			for (auto& layer : m_layers)
			{
				auto* cast = dynamic_cast<Layer*>(&layer);
				if (cast)
				{
					return *cast;
				}
			}
			TD_CORE_ASSERT(false, std::format("RenderLayer '{}' does not exist", typeid(Layer).name()))
		}

	private:
		void Render() const;
		
		template<class Renderer>
		requires(std::is_base_of_v<Component, Renderer>)
		void SetDirty()
		{
			auto it = m_typesToLayers.find(typeid(Renderer));
			if (it == m_typesToLayers.end())
			{
				return;
			}

			for (const auto& index : it->second)
			{
				m_layers[index]->m_dirty = true;
			}
		}

	private:
		std::vector<std::unique_ptr<RenderLayer>> m_layers;
		std::unordered_map<std::type_index, std::vector<size_t>> m_typesToLayers;

		friend class RenderSystem;
	};
}