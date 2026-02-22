#include "tdpch.hpp"
#include "Camera.hpp"
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

namespace TwoD
{
	static Camera* s_Instance = nullptr;
	
	Camera* Camera::Get()
	{
		if (!s_Instance)
		{
			TD_CORE_ERROR("camera is not initialized");
		}
		return s_Instance;
	}

	void Camera::StartBefore()
	{
		s_Instance = this;
		m_window = &App::Get<Window>();
	}

	void Camera::UpdateBefore(float delta)
	{
		int width, height;
		m_window->GetSize(width, height);

		float ratio = static_cast<float>(height) / static_cast<float>(width);

		float right = zoom * 0.5f;
		float left = -right;
		float bottom = zoom * 0.5f * ratio;
		float top = -bottom;

		m_projectionMatrix = glm::ortho(left, right, bottom, top);
		m_inverseProjectionMatrix = glm::inverse(m_projectionMatrix);

		m_projectionMatrixFixedZoom = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);

		auto& transform = GetComponent<Transform>().GetWorldMatrix();
		m_viewMatrix[0][0] = transform[0][0];
		m_viewMatrix[0][1] = transform[0][1];
		m_viewMatrix[1][0] = transform[1][0];
		m_viewMatrix[1][1] = transform[1][1];
		m_viewMatrix[3][0] = transform[2][0];
		m_viewMatrix[3][1] = transform[2][1];
		m_inverseViewMatrix = glm::inverse(m_viewMatrix);

		m_mousePositionWorld = ScreenToWorldSpace(Inputs::GetMousePosition());

		m_projectionViewMatrix = m_projectionMatrix * m_inverseViewMatrix;
	}

	const glm::fvec2 Camera::ScreenToWorldSpace(const glm::fvec2 pos) const
	{
		int width, height;
		m_window->GetSize(width, height);

		glm::fvec4 clipPos = {
			(2.0f * pos.x) / (float)width - 1.0f,
			1.0f - (2.0f * pos.y) / (float)height,
			-1.0f,
			1.0f
		};

		auto worldPos = GetCameraToWorldMatrix() * GetInverseProjectionMatrix() * clipPos;
		worldPos /= worldPos.w;
		return (glm::fvec2)worldPos;
	}
}