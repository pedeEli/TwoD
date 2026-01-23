#include "tdpch.hpp"
#include "TextRenderLayer.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

namespace TwoD
{
	TextRenderLayer::TextRenderLayer()
	{
		m_shader = &App::Get<AssetManager>().Get<Shader>("TwoDLib::TextRenderer");
	}

	void TextRenderLayer::Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const
	{
		auto& renderers = ecs.GetComponents<TextRenderer>();

		m_shader->Bind(&renderPass);

		int width, height;
		App::Get<Window>().GetSize(width, height);

		auto camera = Camera::Get();
		Uniform uniformInWorld{
			camera->GetProjectionMatrix(),
			camera->GetWorldToCameraMatrix(),
			{ 1.0f }
		};
		Uniform uniformOnScreen{
			camera->GetProjectionMatrixFixedZoom(),
			{ 1.0f },
			{ 1.0f }
		};

		size_t i = 0;
		auto data = m_transferBuffer.Map<Instance>(true);
		for (auto index : m_indices)
		{
			auto& renderer = renderers[index];
			renderer.font->Bind(&renderPass);
			const auto& glyphs = renderer.GetGlyphs();

			for (size_t i = 0; i < glyphs.size(); i++)
			{
				auto& glyph = glyphs[i];
				data[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
				data[i].texMin = glyph.texMin;
				data[i].texMax = glyph.texMax;
				data[i].quadMin = glyph.quadMin;
				data[i].quadMax = glyph.quadMax;
			}

			auto size = static_cast<uint32_t>(sizeof(Instance) * glyphs.size());
			auto copyPass = commandBuffer.BeginCopyPass();
			copyPass.UploadToBuffer(
				{ &m_transferBuffer, 0 },
				{ &m_buffer, 0, size },
				true
			);

			Uniform& uniform = renderer.renderLocation == RenderLocation::InWorld ? uniformInWorld : uniformOnScreen;

			auto& model = renderer.GetComponent<Transform>()->GetWorldMatrix();
			uniform.model[0][0] = model[0][0];
			uniform.model[0][1] = model[0][1];
			uniform.model[1][0] = model[1][0];
			uniform.model[1][1] = model[1][1];
			uniform.model[3][0] = model[2][0];
			uniform.model[3][1] = model[2][1];

			renderPass.BindVertexStorageBuffers(0, { &m_buffer });
			commandBuffer.PushVertexUniformData<Uniform>(0, uniform);
			renderPass.DrawPrimitives(glyphs.size() * 6, 1, 0, 0);
		}
	}

	void TextRenderLayer::Update(ECS& ecs, Window& window)
	{
		auto& renderers = ecs.GetComponents<TextRenderer>();
		auto size = renderers.size();
		if (m_indices.size() < size)
		{
			m_indices.reserve(size);
			for (size_t i = m_indices.size(); i < size; i++)
			{
				m_indices.push_back(i);
			}
		}
		else if (m_indices.size() > size)
		{
			m_indices.erase(std::find_if(m_indices.begin(), m_indices.end(), [size](size_t index)
				{
					return index >= size;
				}));
		}
		std::sort(m_indices.begin(), m_indices.end(), [&renderers](auto a, auto b)
			{
				auto layerA = renderers[a].layer;
				auto layerB = renderers[b].layer;
				return layerA < layerB;
			});

		uint32_t bufferSize = 0;
		for (auto& renderer : renderers)
		{
			bufferSize = std::max(bufferSize, static_cast<uint32_t>(renderer.GetGlyphs().size()));
		}

		if (bufferSize != m_bufferSize)
		{
			uint32_t size = bufferSize * sizeof(Instance);
			m_buffer = window.CreateBuffer({ SDL::BufferUsage::GRAPHICS_STORAGE_READ, size });
			m_transferBuffer = window.CreateTransferBuffer({ SDL::TransferBufferUsage::UPLOAD, size });
			m_bufferSize = bufferSize;
		}
	}

	const std::vector<std::type_index>& TextRenderLayer::GetRendererTypes() const
	{
		return s_types;
	}
}