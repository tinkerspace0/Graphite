#pragma once

namespace Graphite
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define GF_KEY_SPACE           ::Graphite::Key::Space
#define GF_KEY_APOSTROPHE      ::Graphite::Key::Apostrophe    /* ' */
#define GF_KEY_COMMA           ::Graphite::Key::Comma         /* , */
#define GF_KEY_MINUS           ::Graphite::Key::Minus         /* - */
#define GF_KEY_PERIOD          ::Graphite::Key::Period        /* . */
#define GF_KEY_SLASH           ::Graphite::Key::Slash         /* / */
#define GF_KEY_0               ::Graphite::Key::D0
#define GF_KEY_1               ::Graphite::Key::D1
#define GF_KEY_2               ::Graphite::Key::D2
#define GF_KEY_3               ::Graphite::Key::D3
#define GF_KEY_4               ::Graphite::Key::D4
#define GF_KEY_5               ::Graphite::Key::D5
#define GF_KEY_6               ::Graphite::Key::D6
#define GF_KEY_7               ::Graphite::Key::D7
#define GF_KEY_8               ::Graphite::Key::D8
#define GF_KEY_9               ::Graphite::Key::D9
#define GF_KEY_SEMICOLON       ::Graphite::Key::Semicolon     /* ; */
#define GF_KEY_EQUAL           ::Graphite::Key::Equal         /* = */
#define GF_KEY_A               ::Graphite::Key::A
#define GF_KEY_B               ::Graphite::Key::B
#define GF_KEY_C               ::Graphite::Key::C
#define GF_KEY_D               ::Graphite::Key::D
#define GF_KEY_E               ::Graphite::Key::E
#define GF_KEY_F               ::Graphite::Key::F
#define GF_KEY_G               ::Graphite::Key::G
#define GF_KEY_H               ::Graphite::Key::H
#define GF_KEY_I               ::Graphite::Key::I
#define GF_KEY_J               ::Graphite::Key::J
#define GF_KEY_K               ::Graphite::Key::K
#define GF_KEY_L               ::Graphite::Key::L
#define GF_KEY_M               ::Graphite::Key::M
#define GF_KEY_N               ::Graphite::Key::N
#define GF_KEY_O               ::Graphite::Key::O
#define GF_KEY_P               ::Graphite::Key::P
#define GF_KEY_Q               ::Graphite::Key::Q
#define GF_KEY_R               ::Graphite::Key::R
#define GF_KEY_S               ::Graphite::Key::S
#define GF_KEY_T               ::Graphite::Key::T
#define GF_KEY_U               ::Graphite::Key::U
#define GF_KEY_V               ::Graphite::Key::V
#define GF_KEY_W               ::Graphite::Key::W
#define GF_KEY_X               ::Graphite::Key::X
#define GF_KEY_Y               ::Graphite::Key::Y
#define GF_KEY_Z               ::Graphite::Key::Z
#define GF_KEY_LEFT_BRACKET    ::Graphite::Key::LeftBracket   /* [ */
#define GF_KEY_BACKSLASH       ::Graphite::Key::Backslash     /* \ */
#define GF_KEY_RIGHT_BRACKET   ::Graphite::Key::RightBracket  /* ] */
#define GF_KEY_GRAVE_ACCENT    ::Graphite::Key::GraveAccent   /* ` */
#define GF_KEY_WORLD_1         ::Graphite::Key::World1        /* non-US #1 */
#define GF_KEY_WORLD_2         ::Graphite::Key::World2        /* non-US #2 */

/* Function keys */
#define GF_KEY_ESCAPE          ::Graphite::Key::Escape
#define GF_KEY_ENTER           ::Graphite::Key::Enter
#define GF_KEY_TAB             ::Graphite::Key::Tab
#define GF_KEY_BACKSPACE       ::Graphite::Key::Backspace
#define GF_KEY_INSERT          ::Graphite::Key::Insert
#define GF_KEY_DELETE          ::Graphite::Key::Delete
#define GF_KEY_RIGHT           ::Graphite::Key::Right
#define GF_KEY_LEFT            ::Graphite::Key::Left
#define GF_KEY_DOWN            ::Graphite::Key::Down
#define GF_KEY_UP              ::Graphite::Key::Up
#define GF_KEY_PAGE_UP         ::Graphite::Key::PageUp
#define GF_KEY_PAGE_DOWN       ::Graphite::Key::PageDown
#define GF_KEY_HOME            ::Graphite::Key::Home
#define GF_KEY_END             ::Graphite::Key::End
#define GF_KEY_CAPS_LOCK       ::Graphite::Key::CapsLock
#define GF_KEY_SCROLL_LOCK     ::Graphite::Key::ScrollLock
#define GF_KEY_NUM_LOCK        ::Graphite::Key::NumLock
#define GF_KEY_PRINT_SCREEN    ::Graphite::Key::PrintScreen
#define GF_KEY_PAUSE           ::Graphite::Key::Pause
#define GF_KEY_F1              ::Graphite::Key::F1
#define GF_KEY_F2              ::Graphite::Key::F2
#define GF_KEY_F3              ::Graphite::Key::F3
#define GF_KEY_F4              ::Graphite::Key::F4
#define GF_KEY_F5              ::Graphite::Key::F5
#define GF_KEY_F6              ::Graphite::Key::F6
#define GF_KEY_F7              ::Graphite::Key::F7
#define GF_KEY_F8              ::Graphite::Key::F8
#define GF_KEY_F9              ::Graphite::Key::F9
#define GF_KEY_F10             ::Graphite::Key::F10
#define GF_KEY_F11             ::Graphite::Key::F11
#define GF_KEY_F12             ::Graphite::Key::F12
#define GF_KEY_F13             ::Graphite::Key::F13
#define GF_KEY_F14             ::Graphite::Key::F14
#define GF_KEY_F15             ::Graphite::Key::F15
#define GF_KEY_F16             ::Graphite::Key::F16
#define GF_KEY_F17             ::Graphite::Key::F17
#define GF_KEY_F18             ::Graphite::Key::F18
#define GF_KEY_F19             ::Graphite::Key::F19
#define GF_KEY_F20             ::Graphite::Key::F20
#define GF_KEY_F21             ::Graphite::Key::F21
#define GF_KEY_F22             ::Graphite::Key::F22
#define GF_KEY_F23             ::Graphite::Key::F23
#define GF_KEY_F24             ::Graphite::Key::F24
#define GF_KEY_F25             ::Graphite::Key::F25

/* Keypad */
#define GF_KEY_KP_0            ::Graphite::Key::KP0
#define GF_KEY_KP_1            ::Graphite::Key::KP1
#define GF_KEY_KP_2            ::Graphite::Key::KP2
#define GF_KEY_KP_3            ::Graphite::Key::KP3
#define GF_KEY_KP_4            ::Graphite::Key::KP4
#define GF_KEY_KP_5            ::Graphite::Key::KP5
#define GF_KEY_KP_6            ::Graphite::Key::KP6
#define GF_KEY_KP_7            ::Graphite::Key::KP7
#define GF_KEY_KP_8            ::Graphite::Key::KP8
#define GF_KEY_KP_9            ::Graphite::Key::KP9
#define GF_KEY_KP_DECIMAL      ::Graphite::Key::KPDecimal
#define GF_KEY_KP_DIVIDE       ::Graphite::Key::KPDivide
#define GF_KEY_KP_MULTIPLY     ::Graphite::Key::KPMultiply
#define GF_KEY_KP_SUBTRACT     ::Graphite::Key::KPSubtract
#define GF_KEY_KP_ADD          ::Graphite::Key::KPAdd
#define GF_KEY_KP_ENTER        ::Graphite::Key::KPEnter
#define GF_KEY_KP_EQUAL        ::Graphite::Key::KPEqual

#define GF_KEY_LEFT_SHIFT      ::Graphite::Key::LeftShift
#define GF_KEY_LEFT_CONTROL    ::Graphite::Key::LeftControl
#define GF_KEY_LEFT_ALT        ::Graphite::Key::LeftAlt
#define GF_KEY_LEFT_SUPER      ::Graphite::Key::LeftSuper
#define GF_KEY_RIGHT_SHIFT     ::Graphite::Key::RightShift
#define GF_KEY_RIGHT_CONTROL   ::Graphite::Key::RightControl
#define GF_KEY_RIGHT_ALT       ::Graphite::Key::RightAlt
#define GF_KEY_RIGHT_SUPER     ::Graphite::Key::RightSuper
#define GF_KEY_MENU            ::Graphite::Key::Menu