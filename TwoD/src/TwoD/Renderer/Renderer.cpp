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
	void Renderer::Shutdown()
	{
		m_vertexBuffer.Release();
		m_vertexTransferBuffer.Release();
		m_indexBuffer.Release();
		m_indexTransferBuffer.Release();
		m_dummyTexture.Release();
		m_dummySampler.Release();
	}

	void Renderer::Render(
		const SDL::CommandBuffer& commandBuffer,
		const SDL::RenderPass& renderPass,
		const std::vector<RenderHandlerInfo>& infos,
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
			if (
				m_currentRenderCommand.handlerIndex != info.handlerIndex ||
				m_currentRenderCommand.projection != info.projection ||
				m_currentRenderCommand.scissorRect != info.scissorRect
			)
			{
				if (m_currentRenderCommand.indexSize != 0)
				{
					m_renderCommands.push_back(m_currentRenderCommand);
				}
				m_currentRenderCommand.handlerIndex = info.handlerIndex;
				m_currentRenderCommand.vertexStartIndex = m_vertexBufferIndex;
				m_currentRenderCommand.vertexSize = 0;
				m_currentRenderCommand.indexStartIndex = m_indexBufferIndex;
				m_currentRenderCommand.indexSize = 0;
				m_currentRenderCommand.projection = info.projection;
				m_currentRenderCommand.scissorRect = info.scissorRect;
			}
			(*handlers)[info.handlerIndex]->Render(*this, info.rendererIndex);
		}
		NextBatch();
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
		if (m_vertexBufferIndex + 4 >= s_maxNumberOfVertices || m_indexBufferIndex + 6 >= s_maxNumberOfIndices)
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
			m_vertexBufferPtr[m_vertexBufferIndex + i].pos.x = transformed.x;
			m_vertexBufferPtr[m_vertexBufferIndex + i].pos.y = transformed.y;
			m_vertexBufferPtr[m_vertexBufferIndex + i].tex = texture[i];
			m_vertexBufferPtr[m_vertexBufferIndex + i].color = color;
		}

		m_indexBufferPtr[m_indexBufferIndex++] = static_cast<uint32_t>(m_vertexBufferIndex) + 0;
		m_indexBufferPtr[m_indexBufferIndex++] = static_cast<uint32_t>(m_vertexBufferIndex) + 1;
		m_indexBufferPtr[m_indexBufferIndex++] = static_cast<uint32_t>(m_vertexBufferIndex) + 2;
		m_indexBufferPtr[m_indexBufferIndex++] = static_cast<uint32_t>(m_vertexBufferIndex) + 3;
		m_indexBufferPtr[m_indexBufferIndex++] = static_cast<uint32_t>(m_vertexBufferIndex) + 2;
		m_indexBufferPtr[m_indexBufferIndex++] = static_cast<uint32_t>(m_vertexBufferIndex) + 1;
		
		m_vertexBufferIndex += 4;
		m_currentRenderCommand.vertexSize += 4;
		m_currentRenderCommand.indexSize += 6;
	}

	void Renderer::RenderMesh(
		const glm::fmat3x3& transform,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices,
		const glm::fvec4& color
	)
	{
		if (m_vertexBufferIndex + vertices.size() >= s_maxNumberOfVertices || m_indexBufferIndex + indices.size() >= s_maxNumberOfIndices)
		{
			NextBatch();
		}

		for (size_t i = 0; i < vertices.size(); i++)
		{
			auto& vertex = vertices[i];
			auto& buffer = m_vertexBufferPtr[m_vertexBufferIndex + i];
			buffer.pos = transform * glm::fvec3(vertex.pos, 1.0f);
			buffer.tex = vertex.tex;
			buffer.color = vertex.color * color;
		}

		for (size_t i = 0; i < indices.size(); i++)
		{
			m_indexBufferPtr[m_indexBufferIndex + i] = static_cast<uint32_t>(m_vertexBufferIndex + indices[i]);
		}

		m_vertexBufferIndex += vertices.size();
		m_currentRenderCommand.vertexSize += vertices.size();
		m_indexBufferIndex += indices.size();
		m_currentRenderCommand.indexSize += indices.size();
	}
	void Renderer::RenderMesh(
		const glm::fmat3x3& transform,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices,
		const TextureBinding& binding,
		const glm::fvec4& color
	)
	{
		TD_CORE_ASSERT(m_textureBindings[binding.slot] == &m_dummyBinding || m_textureBindings[binding.slot] == binding.binding, "Cannot assign different texture to same slot.");
		if (m_textureBindings[binding.slot] == &m_dummyBinding)
		{
			m_textureBindings[binding.slot] = binding.binding;
		}
		RenderMesh(transform, vertices, indices, color);
	}

	void Renderer::NextBatch()
	{
		if (m_currentRenderCommand.indexSize != 0)
		{
			m_renderCommands.push_back(m_currentRenderCommand);
		}
		m_currentRenderCommand.vertexStartIndex = 0;
		m_currentRenderCommand.vertexSize = 0;
		m_currentRenderCommand.indexStartIndex = 0;
		m_currentRenderCommand.indexSize = 0;
		m_vertexBufferIndex = 0;
		m_indexBufferIndex = 0;

		auto vertexCopyPass = m_commandBuffer->BeginCopyPass();
		vertexCopyPass.UploadToBuffer(
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
		vertexCopyPass.End();
		auto indexCopyPass = m_commandBuffer->BeginCopyPass();
		indexCopyPass.UploadToBuffer(
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
		indexCopyPass.End();

		for (size_t i = 0; i < m_textureBindings.size(); i++)
		{
			auto* binding = m_textureBindings[i];
			m_renderPass->BindFragmentSamplers(static_cast<uint32_t>(i), { *binding });
		}

		m_renderPass->BindVertexBuffers(0, { { &m_vertexBuffer, 0 } });
		m_renderPass->BindIndexBuffer({ &m_indexBuffer, 0 }, SDL::IndexElementSize::THIRTY_TWO_BIT);

		const glm::fmat4x4* currentProjection = nullptr;
		std::optional<Rect<float>> scissorRect;
		for (auto& renderCommand : m_renderCommands)
		{
			if (currentProjection != renderCommand.projection)
			{
				currentProjection = renderCommand.projection;
				m_commandBuffer->PushVertexUniformData<glm::fmat4x4>(0, *currentProjection);
			}
			if (
				(!scissorRect && renderCommand.scissorRect) ||
				(scissorRect && !renderCommand.scissorRect) ||
				(scissorRect && *scissorRect != *renderCommand.scissorRect)
			)
			{ 
				scissorRect = renderCommand.scissorRect;
				if (scissorRect) 
				{
					m_renderPass->SetScissorRect(scissorRect);
				}
				else
				{
					m_renderPass->SetScissorRect({});
				}
			}
			(*m_handlers)[renderCommand.handlerIndex]->Bind(m_commandBuffer, m_renderPass);
			m_renderPass->DrawIndexedPrimitives(
				static_cast<uint32_t>(renderCommand.indexSize),
				1,
				static_cast<uint32_t>(renderCommand.indexStartIndex),
				0,
				0
			);
		}

		m_renderCommands.clear();
	}
}