#include "tdpch.hpp"
#include "InputSystem.hpp"

namespace TwoD
{
	void InputSystem::Event(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_KEY_DOWN:
			if (!event.key.repeat)
			{
				m_downKeys.insert(event.key.key);
			}
			break;
		case SDL_EVENT_KEY_UP:
			m_downKeys.erase(event.key.key);
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			m_mouseWheel = event.wheel.y;
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			m_downButtons.insert(event.button.button);
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			m_downButtons.erase(event.button.button);
			break;
		}
	}

	void InputSystem::Update()
	{
		m_mouseWheel = 0.0f;
		SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
	}

	bool InputSystem::GetButtonDown(SDL_Keycode code) const
	{
		return m_downKeys.contains(code);
	}
	bool InputSystem::GetMouseDown(uint8_t button) const
	{
		return m_downButtons.contains(button);
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