#pragma once
#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>
#include <unordered_set>
#include "InputDefines.hpp"

namespace TwoD
{
	class InputSystem
	{
	public:
		InputSystem() = default;
		~InputSystem() = default;
		InputSystem(InputSystem& other) = delete;
		InputSystem(InputSystem&& other) = delete;
		InputSystem& operator=(InputSystem& other) = delete;
		InputSystem& operator=(InputSystem&& other) = delete;

		void Init();
		void Update();
		void Event(const SDL_Event& event);

		bool GetButtonDown(Scancode code) const;
		bool GetMouseDown(MouseButton button) const;
		const glm::fvec2& GetMousePosition() const;
		float GetWheel() const;

	private:
		glm::fvec2 m_mousePosition = { 0.0f, 0.0f };
		float m_mouseWheel;
		MouseButton m_mouseState = MouseButton::NONE;
		const bool* m_keyboardState = nullptr;
	};
}