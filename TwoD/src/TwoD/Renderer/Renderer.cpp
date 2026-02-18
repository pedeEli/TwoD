#include "tdpch.hpp"
#include "Renderer.hpp"

#include "TwoD/Core/App.hpp"



namespace TwoD
{
	void Renderer::Init(const Window& window)
	{
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

	void Renderer::Render(
		const SDL::CommandBuffer& commandBuffer,
		const SDL::RenderPass& renderPass,
		const std::vector<RendererHandlerInfo>& infos,
		const std::vector<std::unique_ptr<RenderHandler>>* handlers
	)
	{
		m_handlers = handlers;
		m_commandBuffer = &commandBuffer;
		m_renderPass = &renderPass;

		for (auto& textureBinding : m_textureBindings)
		{
			textureBinding = &m_dummyBinding;
		}

		m_currentRenderCommand = {};
		for (auto& info : infos)
		{
			if (m_currentRenderCommand.handlerIndex != info.handlerIndex || m_currentRenderCommand.projection != info.projection)
			{
				if (m_currentRenderCommand.size != 0)
				{
					m_renderCommands.push_back(m_currentRenderCommand);
				}
				m_currentRenderCommand.handlerIndex = info.handlerIndex;
				m_currentRenderCommand.startIndex += m_currentRenderCommand.size;
				m_currentRenderCommand.size = 0;
				m_currentRenderCommand.projection = info.projection;
			}
			(*handlers)[info.handlerIndex]->Render(*this, info.rendererIndex);
		}
		NextBatch();
		m_renderCommands.clear();
	}

	void Renderer::RenderQuad(
		const glm::fmat3x3& transform,
		const glm::fvec2& pos,
		const glm::fvec2& size,
		const glm::fvec4& color
	)
	{
		RenderQuad(transform, pos, size, { 0.0f, 0.0f }, { 0.0f, 0.0f }, color);
	}
	void Renderer::RenderQuad(
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

	void Renderer::RenderQuad(
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

		auto indexBase = static_cast<uint32_t>(m_quadIndex * 4);
		m_indexBufferPtr[m_quadIndex * 6 + 0] = indexBase + 0;
		m_indexBufferPtr[m_quadIndex * 6 + 1] = indexBase + 1;
		m_indexBufferPtr[m_quadIndex * 6 + 2] = indexBase + 2;
		m_indexBufferPtr[m_quadIndex * 6 + 3] = indexBase + 3;
		m_indexBufferPtr[m_quadIndex * 6 + 4] = indexBase + 2;
		m_indexBufferPtr[m_quadIndex * 6 + 5] = indexBase + 1;

		m_quadIndex++;

		m_currentRenderCommand.size++;
	}

	void Renderer::NextBatch()
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
			m_renderPass->BindFragmentSamplers(static_cast<uint32_t>(i), { *binding });
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
			(*m_handlers)[renderCommand.handlerIndex]->Bind(m_commandBuffer, m_renderPass);
			m_renderPass->DrawIndexedPrimitives(
				static_cast<uint32_t>(renderCommand.size * 6),
				1,
				static_cast<uint32_t>(renderCommand.startIndex * 6),
				0,
				0
			);
		}

		m_quadIndex = 0;
	}
}