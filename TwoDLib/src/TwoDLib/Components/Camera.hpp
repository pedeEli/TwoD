#pragma once
#include "TwoD/Core/App.hpp"
#include "TwoD/ECS/Transform.hpp"

namespace TwoD
{
	class Camera : public Component
	{
		using Component::Component;
	public:
		static Camera* Get();
	public:
		void StartBefore() override;
		void UpdateBefore(float delta) override;

		const glm::fmat4x4& GetWorldToCameraMatrix() const { return m_inverseViewMatrix; }
		const glm::fmat4x4& GetCameraToWorldMatrix() const { return m_viewMatrix; }
		
		const glm::fmat4x4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::fmat4x4& GetInverseProjectionMatrix() const { return m_inverseProjectionMatrix; }

		const glm::fvec2& GetMousePositionWorld() const { return m_mousePositionWorld; }

		const glm::fvec2 ScreenToWorldSpace(const glm::fvec2 pos) const;

	public:
		TD_COMPONENT_FIELDS(
			TD_FIELD(float, zoom, 40.0f)
		)

	private:
		glm::fmat4x4 m_viewMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		glm::fmat4x4 m_inverseViewMatrix;

		glm::fmat4x4 m_projectionMatrix;
		glm::fmat4x4 m_inverseProjectionMatrix;

		glm::fvec2 m_mousePositionWorld;
		Window* m_window;
	};
}