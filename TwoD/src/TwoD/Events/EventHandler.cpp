#include "tdpch.hpp"
#include "EventHandler.hpp"
#include <SDL3/SDL_events.h>


namespace TwoD
{
	static void ConvertAndEmitEvent(SDL_Event& event, std::function<void(const Event<EventType>&)> emitter)
	{
		switch (event.type)
		{
		{
		case SDL_EVENT_QUIT:
			QuitEvent quitEvent;
			emitter(quitEvent);
			break;
		}

		case SDL_EVENT_KEY_DOWN:
		{
			KeyDownEvent keyDownEvent;
			keyDownEvent.windowID = event.key.windowID;
			keyDownEvent.keyboardID = event.key.which;
			keyDownEvent.key = static_cast<Key>(event.key.key);
			keyDownEvent.mod = static_cast<Keymod>(event.key.mod);
			keyDownEvent.scancode = static_cast<Scancode>(event.key.scancode);
			keyDownEvent.down = true;
			keyDownEvent.repeat = event.key.repeat;
			emitter(keyDownEvent);
			break;
		}
		case SDL_EVENT_KEY_UP:
		{
			KeyUpEvent keyUpEvent;
			keyUpEvent.windowID = event.key.windowID;
			keyUpEvent.keyboardID = event.key.which;
			keyUpEvent.key = static_cast<Key>(event.key.key);
			keyUpEvent.mod = static_cast<Keymod>(event.key.mod);
			keyUpEvent.scancode = static_cast<Scancode>(event.key.scancode);
			keyUpEvent.down = false;
			emitter(keyUpEvent);
			break;
		}
		case SDL_EVENT_TEXT_EDITING:
		{
			TextEditingEvent textEditingEvent;
			textEditingEvent.windowID = event.edit.windowID;
			textEditingEvent.text = event.edit.text;
			textEditingEvent.start = event.edit.start;
			textEditingEvent.length = event.edit.length;
			emitter(textEditingEvent);
			break;
		}
		case SDL_EVENT_TEXT_INPUT:
		{
			TextInputEvent textInputEvent;
			textInputEvent.windowID = event.text.windowID;
			textInputEvent.text = event.text.text;
			emitter(textInputEvent);
			break;
		}

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			MouseDownEvent mouseDownEvent;
			mouseDownEvent.windowID = event.button.windowID;
			mouseDownEvent.mouseID = event.button.which;
			mouseDownEvent.x = event.button.x;
			mouseDownEvent.y = event.button.y;
			mouseDownEvent.button = static_cast<MouseButton>(1u << event.button.button - 1);
			mouseDownEvent.down = true;
			mouseDownEvent.clicks = event.button.clicks;
			emitter(mouseDownEvent);
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			MouseUpEvent mouseUpEvent;
			mouseUpEvent.windowID = event.button.windowID;
			mouseUpEvent.mouseID = event.button.which;
			mouseUpEvent.x = event.button.x;
			mouseUpEvent.y = event.button.y;
			mouseUpEvent.button = static_cast<MouseButton>(1u << event.button.button);
			mouseUpEvent.down = false;
			mouseUpEvent.clicks = event.button.clicks;
			emitter(mouseUpEvent);
			break;
		}
		case SDL_EVENT_MOUSE_MOTION:
		{
			MouseMotionEvent mouseMotionEvent;
			mouseMotionEvent.windowID = event.motion.windowID;
			mouseMotionEvent.mouseID = event.motion.which;
			mouseMotionEvent.x = event.motion.x;
			mouseMotionEvent.y = event.motion.y;
			mouseMotionEvent.mouseState = static_cast<MouseButton>(event.motion.state);
			mouseMotionEvent.xrel = event.motion.xrel;
			mouseMotionEvent.yrel = event.motion.yrel;
			emitter(mouseMotionEvent);
			break;
		}
		case SDL_EVENT_MOUSE_WHEEL:
		{
			MouseWheelEvent mouseWheelEvent;
			mouseWheelEvent.windowID = event.wheel.windowID;
			mouseWheelEvent.mouseID = event.wheel.which;
			mouseWheelEvent.x = event.wheel.x;
			mouseWheelEvent.y = event.wheel.y;
			mouseWheelEvent.direction = static_cast<MouseWheelDirection>(event.wheel.direction);
			mouseWheelEvent.mouseX = event.wheel.mouse_x;
			mouseWheelEvent.mouseY = event.wheel.mouse_y;
			mouseWheelEvent.scrollTicksX = event.wheel.integer_x;
			mouseWheelEvent.scrollTicksY = event.wheel.integer_y;
			emitter(mouseWheelEvent);
			break;
		}

		case SDL_EVENT_WINDOW_RESIZED:
		{
			WindowResizedEvent windowResizedEvent;
			windowResizedEvent.windowID = event.window.windowID;
			windowResizedEvent.x = event.window.data1;
			windowResizedEvent.y = event.window.data2;
			emitter(windowResizedEvent);
			break;
		}
		}
	}

	void EventHandler::EmitEvent(const Event<EventType>& event)
	{
		auto it = m_callbacks.find(event.GetEventType());
		if (it != m_callbacks.end())
		{
			for (auto& callback : it->second)
			{
				if (callback(event))
				{
					break;
				}
			}
		}
	}

	void EventHandler::PollEvents()
	{
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent))
		{
			ConvertAndEmitEvent(sdlEvent, EventHandler::EmitEvent);
		}
	}
}