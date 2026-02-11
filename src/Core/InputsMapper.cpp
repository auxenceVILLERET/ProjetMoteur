#include "InputsMapper.h"


namespace core
{

    Keyboard::Key core::MapVKToKey(WPARAM vkCode)
    {
        switch (vkCode)
        {
            // ===== Lettres =====
        case 'A': return Keyboard::A;
        case 'B': return Keyboard::B;
        case 'C': return Keyboard::C;
        case 'D': return Keyboard::D;
        case 'E': return Keyboard::E;
        case 'F': return Keyboard::F;
        case 'G': return Keyboard::G;
        case 'H': return Keyboard::H;
        case 'I': return Keyboard::I;
        case 'J': return Keyboard::J;
        case 'K': return Keyboard::K;
        case 'L': return Keyboard::L;
        case 'M': return Keyboard::M;
        case 'N': return Keyboard::N;
        case 'O': return Keyboard::O;
        case 'P': return Keyboard::P;
        case 'Q': return Keyboard::Q;
        case 'R': return Keyboard::R;
        case 'S': return Keyboard::S;
        case 'T': return Keyboard::T;
        case 'U': return Keyboard::U;
        case 'V': return Keyboard::V;
        case 'W': return Keyboard::W;
        case 'X': return Keyboard::X;
        case 'Y': return Keyboard::Y;
        case 'Z': return Keyboard::Z;

            // ===== Chiffres =====
        case '0': return Keyboard::_0;
        case '1': return Keyboard::_1;
        case '2': return Keyboard::_2;
        case '3': return Keyboard::_3;
        case '4': return Keyboard::_4;
        case '5': return Keyboard::_5;
        case '6': return Keyboard::_6;
        case '7': return Keyboard::_7;
        case '8': return Keyboard::_8;
        case '9': return Keyboard::_9;

            // ===== F Keys =====
        case VK_F1: return Keyboard::F1;
        case VK_F2: return Keyboard::F2;
        case VK_F3: return Keyboard::F3;
        case VK_F4: return Keyboard::F4;
        case VK_F5: return Keyboard::F5;
        case VK_F6: return Keyboard::F6;
        case VK_F7: return Keyboard::F7;
        case VK_F8: return Keyboard::F8;
        case VK_F9: return Keyboard::F9;
        case VK_F10: return Keyboard::F10;
        case VK_F11: return Keyboard::F11;
        case VK_F12: return Keyboard::F12;

            // ===== Directions =====
        case VK_LEFT:  return Keyboard::LEFT;
        case VK_RIGHT: return Keyboard::RIGHT;
        case VK_UP:    return Keyboard::UP;
        case VK_DOWN:  return Keyboard::DOWN;

            // ===== Contrôles =====
        case VK_ESCAPE: return Keyboard::ESCAPE;
        case VK_SPACE:  return Keyboard::SPACE;
        case VK_RETURN: return Keyboard::RETURN;
        case VK_BACK:   return Keyboard::BACKSPACE;
        case VK_TAB:    return Keyboard::TAB;

            // ===== Modifiers =====
        case VK_LCONTROL: return Keyboard::LCTRL;
        case VK_RCONTROL: return Keyboard::RCTRL;

        case VK_LSHIFT: return Keyboard::LSHIFT;
        case VK_RSHIFT: return Keyboard::RSHIFT;

        case VK_LMENU: return Keyboard::LALT;
        case VK_RMENU: return Keyboard::RALT;

        case VK_LWIN: return Keyboard::LWINDOW;
        case VK_RWIN: return Keyboard::RWINDOW;

            // ===== Numpad =====
        case VK_NUMPAD0: return Keyboard::NUMPAD0;
        case VK_NUMPAD1: return Keyboard::NUMPAD1;
        case VK_NUMPAD2: return Keyboard::NUMPAD2;
        case VK_NUMPAD3: return Keyboard::NUMPAD3;
        case VK_NUMPAD4: return Keyboard::NUMPAD4;
        case VK_NUMPAD5: return Keyboard::NUMPAD5;
        case VK_NUMPAD6: return Keyboard::NUMPAD6;
        case VK_NUMPAD7: return Keyboard::NUMPAD7;
        case VK_NUMPAD8: return Keyboard::NUMPAD8;
        case VK_NUMPAD9: return Keyboard::NUMPAD9;

        case VK_ADD:      return Keyboard::NUMPAD_ADD;
        case VK_SUBTRACT: return Keyboard::NUMPAD_SUBTRACT;
        case VK_MULTIPLY: return Keyboard::NUMPAD_MULTIPLY;
        case VK_DIVIDE:   return Keyboard::NUMPAD_DIVIDE;
        case VK_DECIMAL:  return Keyboard::NUMPAD_DECIMAL;

        case VK_NUMLOCK: return Keyboard::NUMLOCK;
        case VK_CAPITAL: return Keyboard::CAPSLOCK;
        case VK_SCROLL:  return Keyboard::SCROLL_LOCK;

        case VK_INSERT: return Keyboard::INSERT;
        case VK_DELETE: return Keyboard::DELETE_;
        case VK_HOME:   return Keyboard::HOME;
        case VK_END:    return Keyboard::END;
        case VK_PRIOR:  return Keyboard::PAGE_UP;
        case VK_NEXT:   return Keyboard::PAGE_DOWN;

        default:
            return Keyboard::AMOUNT; // invalide
        }
    }

    Mouse::Button MapMouseButton(UINT msg)
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            return Mouse::LEFT;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            return Mouse::RIGHT;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            return Mouse::MIDDLE;

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        {
            // XBUTTON1 ou 2
            return Mouse::EXTRA_1;
        }

        default:
            return Mouse::AMOUNT;
        }
    }

}