#include "tdpch.hpp"
#include "InputSystem.hpp"
#include "TwoD/Events/EventHandler.hpp"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

namespace TwoD
{
	void InputSystem::Init()
	{
		m_keyboardState = SDL_GetKeyboardState(nullptr);
		EventHandler::On<MouseWheelEvent>([this](auto& event)
			{
				m_mouseWheel = event.y;
				return false;
			});
	}

	void InputSystem::Update()
	{
		m_mouseWheel = 0.0f;
		auto mouseState = SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
		m_mouseState = static_cast<MouseButton>(mouseState);
	}

	bool InputSystem::GetButtonDown(Scancode code) const
	{
		return m_keyboardState[static_cast<size_t>(code)];
	}
	bool InputSystem::GetMouseDown(MouseButton button) const
	{
		return (m_mouseState & button) != MouseButton::NONE;
	}
	const glm::fvec2& InputSystem::GetMousePosition() const
	{
		return m_mousePosition;
	}
	float InputSystem::GetWheel() const
	{
		return m_mouseWheel;
	}
}