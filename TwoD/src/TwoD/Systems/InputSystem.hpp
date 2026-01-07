#pragma once
#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>
#include <unordered_set>

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

		void Update();
		void Event(const SDL_Event& event);

		bool GetButtonDown(SDL_Keycode code) const;
		bool GetMouseDown(uint8_t button) const;
		const glm::fvec2& GetMousePosition() const;
		float GetWheel() const;

	private:
		glm::fvec2 m_mousePosition = { 0.0f, 0.0f };
		std::unordered_set<SDL_Keycode> m_downKeys;
		std::unordered_set<uint8_t> m_downButtons;
		float m_mouseWheel;
	};
}