#include "InputsMethods.h"
#include "InputSystem.h"

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

        int GetMouseX()
        {
            return InputSystem::s_mouseX;
        }

        int GetMouseY()
        {
            return InputSystem::s_mouseY;
        }

        void SetMousePosition(int x, int y)
        {
            InputSystem::SetMousePosition(x, y);
        }

        void Update()
        {
            InputSystem::NewFrame();
        }

    }
}
