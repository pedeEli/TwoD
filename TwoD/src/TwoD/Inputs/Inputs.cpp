#include "tdpch.hpp"
#include "Inputs.hpp"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

namespace TwoD
{
	void Inputs::Init(const Window& window)
	{
		m_keyboardState = SDL_GetKeyboardState(nullptr);
	}

	void Inputs::Update()
	{
		auto mouseState = SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
		m_mouseState = static_cast<MouseButton>(mouseState);
	}

	bool Inputs::GetButtonDown(Scancode code)
	{
		return m_keyboardState[static_cast<size_t>(code)];
	}
	bool Inputs::GetMouseDown(MouseButton button)
	{
		return (m_mouseState & button) != MouseButton::NONE;
	}
	const glm::fvec2& Inputs::GetMousePosition()
	{
		return m_mousePosition;
	}
}