#pragma once
#include "InputsEnums.h"

namespace core
{
    namespace Input
    {
        bool GetKey(Keyboard::Key key);
        bool GetKeyDown(Keyboard::Key key);
        bool GetKeyUp(Keyboard::Key key);

        bool GetMouseButton(Mouse::Button button);
        bool GetMouseButtonDown(Mouse::Button button);
        bool GetMouseButtonUp(Mouse::Button button);

        int GetMouseX();
        int GetMouseY();

        void SetMousePosition(int x, int y);

        void Update();
    }
}
