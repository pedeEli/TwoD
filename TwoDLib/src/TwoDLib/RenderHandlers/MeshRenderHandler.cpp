#include "tdpch.hpp"
#include "MeshRenderHandler.hpp"

namespace TwoD
{
	void MeshRenderHandler::Init()
	{
		m_shader = &AssetManager::Get<Shader>("TwoDLib::ColorRenderer");
	}

	void TwoD::MeshRenderHandler::Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const
	{
		m_shader->Bind(renderPass);
	}

	void MeshRenderHandler::Render(Renderer& renderer, size_t index)
	{
		auto& meshRenderer = GetComponents<MeshRenderer>()[index];
		auto* transform = meshRenderer.GetTransform();

		renderer.RenderMesh(
			transform->GetWorldMatrix(),
			meshRenderer.mesh->vertices,
			meshRenderer.mesh->indices,
			static_cast<glm::fvec4>(meshRenderer.color) / 255.0f
		);
	}
}