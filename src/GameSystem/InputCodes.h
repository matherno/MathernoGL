#pragma once

#include "Types.h"
#include "systemio/Window.h"

#define MOUSE_LEFT GLFW_MOUSE_BUTTON_1
#define MOUSE_RIGHT GLFW_MOUSE_BUTTON_2
#define MOUSE_MIDDLE GLFW_MOUSE_BUTTON_3

#define KEY_SPACE GLFW_KEY_SPACE
#define KEY_ESC GLFW_KEY_ESCAPE
#define KEY_ENTER GLFW_KEY_ENTER
#define KEY_TAB GLFW_KEY_TAB
#define KEY_BACKSPACE GLFW_KEY_BACKSPACE
#define KEY_LSHIFT GLFW_KEY_LEFT_SHIFT
#define KEY_RSHIFT GLFW_KEY_RIGHT_SHIFT
#define KEY_LCTRL GLFW_KEY_LEFT_CONTROL
#define KEY_RCTRL GLFW_KEY_RIGHT_CONTROL
#define KEY_LALT GLFW_KEY_LEFT_ALT
#define KEY_RALT GLFW_KEY_RIGHT_ALT
#define KEY_UP GLFW_KEY_UP
#define KEY_LEFT GLFW_KEY_LEFT
#define KEY_DOWN GLFW_KEY_DOWN
#define KEY_RIGHT GLFW_KEY_RIGHT
#define KEY_HOME GLFW_KEY_HOME
#define KEY_END GLFW_KEY_END
#define KEY_DELETE GLFW_KEY_DELETE
#define KEY_KEYPAD_ADD GLFW_KEY_KP_ADD
#define KEY_KEYPAD_MINUS GLFW_KEY_KP_SUBTRACT

#define KEY_F1 GLFW_KEY_F1
#define KEY_F2 GLFW_KEY_F2
#define KEY_F3 GLFW_KEY_F3
#define KEY_F4 GLFW_KEY_F4
#define KEY_F5 GLFW_KEY_F5
#define KEY_F6 GLFW_KEY_F6
#define KEY_F7 GLFW_KEY_F7
#define KEY_F8 GLFW_KEY_F8
#define KEY_F9 GLFW_KEY_F9
#define KEY_F10 GLFW_KEY_F10
#define KEY_F11 GLFW_KEY_F11
#define KEY_F12 GLFW_KEY_F12

/*
*   If the key code belongs to a character, then this will return that character, otherwise returns 0 ('\0')
*/
static char getCharFromKeyCode(uint code, bool shiftMod)
  {
  switch (code)
    {
    case GLFW_KEY_A:  return shiftMod ? 'A' : 'a';
    case GLFW_KEY_B:  return shiftMod ? 'B' : 'b';
    case GLFW_KEY_C:  return shiftMod ? 'C' : 'c';
    case GLFW_KEY_D:  return shiftMod ? 'D' : 'd';
    case GLFW_KEY_E:  return shiftMod ? 'E' : 'e';
    case GLFW_KEY_F:  return shiftMod ? 'F' : 'f';
    case GLFW_KEY_G:  return shiftMod ? 'G' : 'g';
    case GLFW_KEY_H:  return shiftMod ? 'H' : 'h';
    case GLFW_KEY_I:  return shiftMod ? 'I' : 'i';
    case GLFW_KEY_J:  return shiftMod ? 'J' : 'j';
    case GLFW_KEY_K:  return shiftMod ? 'K' : 'k';
    case GLFW_KEY_L:  return shiftMod ? 'L' : 'l';
    case GLFW_KEY_M:  return shiftMod ? 'M' : 'm';
    case GLFW_KEY_N:  return shiftMod ? 'N' : 'n';
    case GLFW_KEY_O:  return shiftMod ? 'O' : 'o';
    case GLFW_KEY_P:  return shiftMod ? 'P' : 'p';
    case GLFW_KEY_Q:  return shiftMod ? 'Q' : 'q';
    case GLFW_KEY_R:  return shiftMod ? 'R' : 'r';
    case GLFW_KEY_S:  return shiftMod ? 'S' : 's';
    case GLFW_KEY_T:  return shiftMod ? 'T' : 't';
    case GLFW_KEY_U:  return shiftMod ? 'U' : 'u';
    case GLFW_KEY_V:  return shiftMod ? 'V' : 'v';
    case GLFW_KEY_W:  return shiftMod ? 'W' : 'w';
    case GLFW_KEY_X:  return shiftMod ? 'X' : 'x';
    case GLFW_KEY_Y:  return shiftMod ? 'Y' : 'y';
    case GLFW_KEY_Z:  return shiftMod ? 'Z' : 'z';
    case GLFW_KEY_1:  return shiftMod ? '!' : '1';
    case GLFW_KEY_2:  return shiftMod ? '@' : '2';
    case GLFW_KEY_3:  return shiftMod ? '#' : '3';
    case GLFW_KEY_4:  return shiftMod ? '$' : '4';
    case GLFW_KEY_5:  return shiftMod ? '%' : '5';
    case GLFW_KEY_6:  return shiftMod ? '^' : '6';
    case GLFW_KEY_7:  return shiftMod ? '&' : '7';
    case GLFW_KEY_8:  return shiftMod ? '*' : '8';
    case GLFW_KEY_9:  return shiftMod ? '(' : '9';
    case GLFW_KEY_0:  return shiftMod ? ')' : '0';
    case GLFW_KEY_KP_0: return '0';
    case GLFW_KEY_KP_1: return '1';
    case GLFW_KEY_KP_2: return '2';
    case GLFW_KEY_KP_3: return '3';
    case GLFW_KEY_KP_4: return '4';
    case GLFW_KEY_KP_5: return '5';
    case GLFW_KEY_KP_6: return '6';
    case GLFW_KEY_KP_7: return '7';
    case GLFW_KEY_KP_8: return '8';
    case GLFW_KEY_KP_9: return '9';
    case GLFW_KEY_KP_DECIMAL: return '.';
    case GLFW_KEY_MINUS:  return shiftMod ? '_' : '-';
    case GLFW_KEY_EQUAL:  return shiftMod ? '+' : '=';
    case GLFW_KEY_SLASH:  return shiftMod ? '?' : '/';
//    case GLFW_KEY_BACKSLASH:  return shiftMod ? '|' : '\';    // avoiding backslash '\' escape character for sanity
    case GLFW_KEY_LEFT_BRACKET:  return shiftMod ? '{' : '[';
    case GLFW_KEY_RIGHT_BRACKET:  return shiftMod ? '}' : ']';
    case GLFW_KEY_SEMICOLON:  return shiftMod ? ':' : ';';
//    case GLFW_KEY_APOSTROPHE:  return shiftMod ? '"' : '''';       // avoiding this too...
    case GLFW_KEY_COMMA:  return shiftMod ? '<' : ',';
    case GLFW_KEY_PERIOD:  return shiftMod ? '>' : '.';
    case GLFW_KEY_GRAVE_ACCENT:  return shiftMod ? '~' : '`';
    default: return '\0';
    }
  }

static char getCharFromKeyCode(uint code)
  {
  return getCharFromKeyCode(code, true);
  }
