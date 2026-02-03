#include "tdpch.hpp"
#include "RenderSystem2.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void RenderSystem2::Init()
	{
		auto& window = App::Get<Window>();

		SDL::BufferInfo vertexBufferInfo{
			.usage = SDL::BufferUsage::VERTEX,
			.size = s_maxNumberOfVertices * sizeof(Vertex)
		};
		SDL::BufferInfo indexBufferInfo{
			.usage = SDL::BufferUsage::INDEX,
			.size = s_maxNumberOfIndices * sizeof(uint32_t)
		};
		m_vertexBuffer = window.CreateBuffer(vertexBufferInfo);
		m_indexBuffer = window.CreateBuffer(indexBufferInfo);

		SDL::TransferBufferInfo vertexTransferBufferInfo{
			.usage = SDL::TransferBufferUsage::UPLOAD,
			.size = s_maxNumberOfVertices * sizeof(Vertex)
		};
		SDL::TransferBufferInfo indexTransferBufferInfo{
			.usage = SDL::TransferBufferUsage::UPLOAD,
			.size = s_maxNumberOfIndices * sizeof(uint32_t)
		};
		m_vertexTransferBuffer = window.CreateTransferBuffer(vertexTransferBufferInfo);
		m_indexTransferBuffer = window.CreateTransferBuffer(indexTransferBufferInfo);

		m_vertexBufferPtr = m_vertexTransferBuffer.Map<Vertex>(false);
		m_indexBufferPtr = m_indexTransferBuffer.Map<uint32_t>(false);

		for (auto& renderLayer : m_renderLayers)
		{
			renderLayer->Init();
		}

		m_dummyTexture = window.CreateTexture({
			.type = SDL::TextureType::TWO_D,
			.format = SDL::TextureFormat::R8G8B8A8_UNORM,
			.usage = SDL::TextureUsageFlags::SAMPLER,
			.width = 1,
			.height = 1,
			.layerCountOrDepth = 1,
			.numLevels = 1,
			.sampleCount = SDL::SampleCount::ONE
		});
		m_dummySampler = window.CreateSampler({});
		m_dummyBinding = { &m_dummyTexture, &m_dummySampler };
	}

	void RenderSystem2::Render()
	{
		auto& ecs = App::Get<ECS>();
		if (m_dirty)
		{
			Update(ecs);
			m_dirty = false;
		}

		auto& window = App::Get<Window>();

		auto commandBuffer = window.AcquireCommandBuffer();
		auto renderPass = window.BeginRenderPass(&commandBuffer);
		m_commandBuffer = &commandBuffer;
		m_renderPass = &renderPass;

		if (!renderPass.Valid())
		{
			TD_CORE_ERROR("Invalid render pass.");
			return;
		}

		for (auto& textureBinding : m_textureBindings)
		{
			textureBinding = &m_dummyBinding;
		}

		m_currentRenderCommand = {};
		for (auto& index : m_indices)
		{
			if (m_currentRenderCommand.renderLayer != index.renderLayer || m_currentRenderCommand.projection != index.projection)
			{
				if (m_currentRenderCommand.size != 0)
				{
					m_renderCommands.push_back(m_currentRenderCommand);
				}
				m_currentRenderCommand.renderLayer = index.renderLayer;
				m_currentRenderCommand.startIndex += m_currentRenderCommand.size;
				m_currentRenderCommand.size = 0;
				m_currentRenderCommand.projection = index.projection;
			}
			m_renderLayers[index.renderLayer]->Render(ecs, *this, m_renderPass, index.index);
		}
		NextBatch();
		m_renderCommands.clear();
	}

	void RenderSystem2::RenderQuad(
		const glm::fmat3x3& transform,
		const glm::fvec2& pos,
		const glm::fvec2& size,
		const glm::fvec4& color
	)
	{
		RenderQuad(transform, pos, size, { 0.0f, 0.0f }, { 0.0f, 0.0f }, color);
	}
	void RenderSystem2::RenderQuad(
		const glm::fmat3x3& transform,
		const glm::fvec2& pos,
		const glm::fvec2& size,
		const glm::fvec2& tex1,
		const glm::fvec2& tex2,
		const TextureBinding& binding,
		const glm::fvec4& color
	)
	{
		TD_CORE_ASSERT(m_textureBindings[binding.slot] == &m_dummyBinding || m_textureBindings[binding.slot] == binding.binding, "Cannot assign different texture to same slot.");
		if (m_textureBindings[binding.slot] == &m_dummyBinding)
		{
			m_textureBindings[binding.slot] = binding.binding;
		}
		RenderQuad(transform, pos, size, tex1, tex2, color);
	}

	void RenderSystem2::RenderQuad(
		const glm::fmat3x3& transform,
		const glm::fvec2& pos,
		const glm::fvec2& size,
		const glm::fvec2& tex1,
		const glm::fvec2& tex2,
		const glm::fvec4& color
	)
	{
		if (m_quadIndex + 1 == s_maxNumberOfQuads)
		{
			NextBatch();
		}

		const glm::fvec3 vertices[] = {
			{ pos.x, pos.y, 1.0f },
			{ pos.x + size.x, pos.y, 1.0f },
			{ pos.x, pos.y + size.y, 1.0f },
			{ pos.x + size.x, pos.y + size.y, 1.0f }
		};
		const glm::fvec2 texture[] = {
			tex1,
			{ tex2.x, tex1.y },
			{ tex1.x, tex2.y },
			tex2
		};

		for (size_t i = 0; i < 4; i++)
		{
			auto transformed = transform * vertices[i];
			m_vertexBufferPtr[m_quadIndex * 4 + i].pos.x = transformed.x;
			m_vertexBufferPtr[m_quadIndex * 4 + i].pos.y = transformed.y;
			m_vertexBufferPtr[m_quadIndex * 4 + i].tex = texture[i];
			m_vertexBufferPtr[m_quadIndex * 4 + i].color = color;
		}

		auto indexBase = m_quadIndex * 4;
		m_indexBufferPtr[m_quadIndex * 6 + 0] = indexBase + 0;
		m_indexBufferPtr[m_quadIndex * 6 + 1] = indexBase + 1;
		m_indexBufferPtr[m_quadIndex * 6 + 2] = indexBase + 2;
		m_indexBufferPtr[m_quadIndex * 6 + 3] = indexBase + 3;
		m_indexBufferPtr[m_quadIndex * 6 + 4] = indexBase + 2;
		m_indexBufferPtr[m_quadIndex * 6 + 5] = indexBase + 1;

		m_quadIndex++;

		m_currentRenderCommand.size++;
	}

	void RenderSystem2::NextBatch()
	{
		if (m_currentRenderCommand.size != 0)
		{
			m_renderCommands.push_back(m_currentRenderCommand);
		}
		m_currentRenderCommand.size = 0;
		m_currentRenderCommand.startIndex = 0;

		{
			m_commandBuffer->BeginCopyPass().UploadToBuffer(
				{
					.transferBuffer = &m_vertexTransferBuffer,
					.offset = 0
				},
				{
					.buffer = &m_vertexBuffer,
					.offset = 0,
					.size = s_maxNumberOfVertices * sizeof(Vertex)
				},
				false
			);
			m_commandBuffer->BeginCopyPass().UploadToBuffer(
				{
					.transferBuffer = &m_indexTransferBuffer,
					.offset = 0
				},
				{
					.buffer = &m_indexBuffer,
					.offset = 0,
					.size = s_maxNumberOfIndices * sizeof(uint32_t)
				},
				false
			);
		}

		for (size_t i = 0; i < m_textureBindings.size(); i++)
		{
			auto* binding = m_textureBindings[i];
			m_renderPass->BindFragmentSamplers(i, { *binding });
		}

		m_renderPass->BindVertexBuffers(0, { { &m_vertexBuffer, 0 } });
		m_renderPass->BindIndexBuffer({ &m_indexBuffer, 0 }, SDL::IndexElementSize::THIRTY_TWO_BIT);

		const glm::fmat4x4* currentProjection = nullptr;
		for (auto& renderCommand : m_renderCommands)
		{
			if (currentProjection != renderCommand.projection)
			{
				currentProjection = renderCommand.projection;
				m_commandBuffer->PushVertexUniformData<glm::fmat4x4>(0, *currentProjection);
			}
			m_renderLayers[renderCommand.renderLayer]->Bind(m_commandBuffer, m_renderPass);
			m_renderPass->DrawIndexedPrimitives(renderCommand.size * 6, 1, renderCommand.startIndex * 6, 0, 0);
		}

		m_quadIndex = 0;
	}

	void RenderSystem2::Update(const ECS& ecs)
	{
		struct LayerInfo
		{
			const std::vector<RenderLayer2::IndexLayer>* layer;
			size_t index;
			size_t renderLayer;

			bool operator==(const LayerInfo& b) const
			{
				return layer == b.layer && index == b.index && renderLayer == b.renderLayer;
			}
		};

		std::vector<LayerInfo> layers;
		layers.reserve(m_renderLayers.size());
		size_t totalSize = 0;
		for (size_t i = 0; i < m_renderLayers.size(); i++)
		{
			m_renderLayers[i]->Update(ecs);
			auto& layer = m_renderLayers[i]->m_indexLayers;
			if (layer.size() == 0)
			{
				continue;
			}
			totalSize += layer.size();
			layers.push_back({ &layer, 0, i });
		}

		m_indices.clear();
		m_indices.reserve(totalSize);

		for (size_t i = 0; i < totalSize; i++)
		{
			auto* smallestLayer = &layers[0];
			auto* smallestIndex = &(*layers[0].layer)[layers[0].index];
			for (size_t j = 1; j < layers.size(); j++)
			{
				auto* currentIndex = &(*layers[j].layer)[layers[j].index];
				if (currentIndex->layer < smallestIndex->layer)
				{
					smallestIndex = currentIndex;
					smallestLayer = &layers[j];
				}
			}

			m_indices.emplace_back(smallestLayer->renderLayer, smallestIndex->index, smallestIndex->layer, smallestIndex->projection);
			smallestLayer->index++;
			if (smallestLayer->index == smallestLayer->layer->size())
			{
				auto it = std::find(layers.begin(), layers.end(), *smallestLayer);
				layers.erase(it);
			}
		}
	}
}