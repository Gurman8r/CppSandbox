#ifndef _KEY_CODE_H_
#define _KEY_CODE_H_

#include <inttypes.h>

// https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes

namespace ml
{
	enum KeyCode : uint32_t
	{
		LeftMouse	= 0x01, RightMouse, Cancel, MiddleMouse, XButton1, XButton2,
		// 0x07 Undefined
		Backspace	= 0x08, Tab, 
		// 0x0A-0B Reserved
		Clear		= 0x0C, Return,
		// 0x0E-0F Undefined
		Shift		= 0x10, Control, Alt, Pause, CapsLock, Kana, Hanguel, Hangul,
		// 0x16 Undefined
		Junja		= 0x17, Final, Hanja, Kanji,
		// 0x1A Undefined
		Escape		= 0x1B, Convert, NonConvert, Accept, ModeChange, 
							Space, PageUp, PageDown, End, Home, Left, Up, Right, Down, 
							Select, Print, Execute, Snapshot, Insert, Delete, Help,
		Num0		= 0x30, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,		
		// 0x3A-40 Undefined
		A			= 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		LeftWindows	= 0x5B, RightWindows, Applications,
		// 0x5E Reserved
		Sleep		= 0x5F,
		NUM_0		= 0x60, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, 
							NUM_Multiply, NUM_Add, NUM_Separator, 
							NUM_Subtract, NUM_Decimal, NUM_Divide,
		// 
		F1			= 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, 
							F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
		// 0x88-8F Unasigned
		NumLock		= 0x90, ScrollLock,
		// 0x92-96 OEM Specific
		// 0x97-9F Unassigned
		LeftShift	= 0xA0, RightShift, LeftControl, RightControl, LeftMenu, RightMenu, 
							BrowserBack, BrowserForward, BrowserRefresh, BrowserStop, BrowserSearch, BrowserHome, BrowserMute,
							VolumeDown, VolumeUp, MediaNext, MediaPrev, MediaStop, MediaPlayPause,
							LaunchMail, LaunchMediaSelect, LaunchApp1, LaunchApp2,
		// 0xB8-B9 Reserved
		OEM_1		= 0xBA, OEM_Plus, OEM_Comma, OEM_Minus, OEM_Period, OEM_2, OEM_3,
		// 0xC1-D7 Reserved
		// 0xD8-Da Unassigned
		OEM_4		= 0xDB, OEM_5, OEM_7, OEM_8,
		// 0xE0 Reserved
		// 0xE1 OEM Specific
		OEM_102		= 0xE2,
		// 0xE3-E4 OEM Specific
		ProcessKey	= 0xE5,
		// 0xE6 OEM Specific
		Packet		= 0xE7,
		// 0xE9-F5 OEM Specific
		Attn		= 0xF7, CRSEL, EXSEL, EREOF, Play, Zoom, NoName, OEM_Clear,

		MAX_KEYCODE
	};
}

#endif // !_KEY_CODE_H_
