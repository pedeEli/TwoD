#include "tdpch.hpp"
#include "SpriteRenderLayer.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

namespace TwoD
{
	SpriteRenderLayer::SpriteRenderLayer()
	{
		m_shader = &App::Get<AssetManager>().Get<Shader>("TwoDLib::SpriteRenderer");
		App::Get<AssetManager>().Get<SpriteAtlas>("sprite-atlas").Pack();
	}

	void SpriteRenderLayer::Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const
	{
		auto& renderers = ecs.GetComponents<SpriteRenderer>();
		auto size = static_cast<uint32_t>(sizeof(Instance) * renderers.size());

		auto data = m_transferBuffer.Map<Instance>(true);
		for (size_t i = 0; i < m_indices.size(); i++)
		{
			auto& renderer = renderers[m_indices[i]];

			auto& model = renderer.GetComponent<Transform>()->GetWorldMatrix();
			data[i].model1.x = model[0].x;
			data[i].model1.y = model[0].y;
			data[i].model2.x = model[1].x;
			data[i].model2.y = model[1].y;
			data[i].model3.x = model[2].x;
			data[i].model3.y = model[2].y;

			data[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			if (renderer.slice)
			{
				data[i].tex = renderer.sprite->GetRect(*renderer.slice);
			}
			else
			{
				data[i].tex = renderer.sprite->GetRect();
			}
		}

		if (size == 0)
		{
			return;
		}

		auto copyPass = commandBuffer.BeginCopyPass();
		copyPass.UploadToBuffer(
			{ &m_transferBuffer, 0 },
			{ &m_buffer, 0, size },
			true
		);

		m_shader->Bind(&renderPass);
		renderPass.BindVertexStorageBuffers(0, { &m_buffer });
		auto& atlas = App::Get<AssetManager>().Get<SpriteAtlas>("sprite-atlas");
		atlas.Bind(&renderPass);

		auto camera = Camera::Get();
		Uniform uniform{
			camera->GetProjectionMatrix(),
			camera->GetWorldToCameraMatrix(),
			atlas.GetSize()
		};
		commandBuffer.PushVertexUniformData<Uniform>(0, uniform);

		renderPass.DrawPrimitives(static_cast<uint32_t>(renderers.size() * 6), 1, 0, 0);
	}

	void SpriteRenderLayer::Update(ECS& ecs, Window& window)
	{
		auto& renderers = ecs.GetComponents<SpriteRenderer>();
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

		auto bufferSize = static_cast<uint32_t>(sizeof(Instance) * size);
		m_buffer = window.CreateBuffer({ SDL::BufferUsage::GRAPHICS_STORAGE_READ, bufferSize });
		m_transferBuffer = window.CreateTransferBuffer({ SDL::TransferBufferUsage::UPLOAD, bufferSize });
	}

	const std::vector<std::type_index>& SpriteRenderLayer::GetRendererTypes() const
	{
		return s_types;
	}
}