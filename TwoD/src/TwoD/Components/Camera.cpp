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

		float right = zoom * 0.5f;
		float left = -right;
		float bottom = zoom * 0.5f * (float)height / (float)width;
		float top = -bottom;

		m_projectionMatrix = glm::ortho(left, right, bottom, top);
		m_inverseProjectionMatrix = glm::inverse(m_projectionMatrix);

		auto& transform = GetComponent<Transform>()->GetWorldMatrix();
		m_viewMatrix[0][0] = transform[0][0];
		m_viewMatrix[0][1] = transform[0][1];
		m_viewMatrix[1][0] = transform[1][0];
		m_viewMatrix[1][1] = transform[1][1];
		m_viewMatrix[3][0] = transform[2][0];
		m_viewMatrix[3][1] = transform[2][1];
		m_inverseViewMatrix = glm::inverse(m_viewMatrix);

		m_mousePositionWorld = ScreenToWorldSpace(App::Get<InputSystem>().GetMousePosition());
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