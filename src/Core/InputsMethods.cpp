#include "InputsMethods.h"
#include "InputSystem.h"
#include <vector>
#include <iostream>

namespace core
{
	namespace Input
	{

		bool GetKey(Keyboard::Key key)
		{
			return InputSystem::s_keys[key] == InputState::PRESSED;
		}

		bool GetKeyDown(Keyboard::Key key)
		{
			return InputSystem::s_keys[key] == InputState::DOWN;
		}

		bool GetKeyUp(Keyboard::Key key)
		{
			return InputSystem::s_keys[key] == InputState::UP;
		}

		bool GetMouseButton(Mouse::Button button)
		{
			return InputSystem::s_mouseButtons[button] == InputState::PRESSED;
		}

		bool GetMouseButtonDown(Mouse::Button button)
		{
			return InputSystem::s_mouseButtons[button] == InputState::DOWN;
		}

		bool GetMouseButtonUp(Mouse::Button button)
		{
			return InputSystem::s_mouseButtons[button] == InputState::UP;
		}

		float GetMouseX()
		{
			return InputSystem::s_mouseX;
		}

		float GetMouseY()
		{
			return InputSystem::s_mouseY;
		}

		std::vector<float> GetMousePosition()
		{
			return { InputSystem::s_mouseX, InputSystem::s_mouseY };
		}

		std::vector<float> GetMouseDelta()
		{
			std::vector<float> delta = { 0.0f,0.0f };
			std::vector<float> current = GetMousePosition();

			if (m_mousePosLastFrame == current || m_mousePosLastFrame.size() < 1) { m_mousePosLastFrame = current; return delta; }
			delta = { current[0] - m_mousePosLastFrame[0] , current[1] - m_mousePosLastFrame[1] };
			m_mousePosLastFrame = current;

			return delta;
		}

		void SetMousePosition(float x, float y)
		{
			InputSystem::SetMousePosition(x, y);
		}

		void Update()
		{
			InputSystem::NewFrame();
		}

	}
}
