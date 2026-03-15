#pragma once
#include <glm/glm.hpp>
#include <unordered_set>
#include "InputDefines.hpp"
#include "TwoD/SDL/Window.hpp"

namespace TwoD
{
	class Inputs
	{
	public:
		static void Init();
		static void Update();

		static bool GetButtonDown(Scancode code);
		static bool GetMouseDown(MouseButton button);
		static const glm::fvec2& GetMousePosition();
		
	private:
		static inline glm::fvec2 m_mousePosition = { 0.0f, 0.0f };
		static inline MouseButton m_mouseState = MouseButton::NONE;
		static inline const bool* m_keyboardState = nullptr;
	};
}