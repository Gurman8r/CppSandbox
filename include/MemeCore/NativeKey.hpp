#ifndef _NATIVE_KEY_H_
#define _NATIVE_KEY_H_

#include <MemeCore/Export.hpp>
#include <inttypes.h>

namespace ml
{
	struct ML_CORE_API NativeKey final
	{
#ifdef ML_SYSTEM_WINDOWS
		enum : int32_t
		{
			LeftMouse	= 0x01, RightMouse, Cancel, MiddleMouse, XButton1, XButton2,
			Backspace	= 0x08, Tab, 
			Clear		= 0x0C, Return,
			Shift		= 0x10, Control, Alt, Pause, CapsLock, Kana, Hanguel, Hangul,
			Junja		= 0x17, Final, Hanja, Kanji,
			Escape		= 0x1B, Convert, NonConvert, Accept, ModeChange, 
								Space, PageUp, PageDown, End, Home, Left, Up, Right, Down, 
								Select, Print, Execute, Snapshot, Insert, Delete, Help,
			Num0		= 0x30, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,		
			A			= 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			LeftWindows	= 0x5B, RightWindows, Applications,
			Sleep		= 0x5F,
			NUM_0		= 0x60, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, 
								NUM_Multiply, NUM_Add, NUM_Separator, 
								NUM_Subtract, NUM_Decimal, NUM_Divide,
			F1			= 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, 
								F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
			NumLock		= 0x90, ScrollLock,
			LeftShift	= 0xA0, RightShift, LeftControl, RightControl, LeftMenu, RightMenu, 
								BrowserBack, BrowserForward, BrowserRefresh, BrowserStop, BrowserSearch, BrowserHome, BrowserMute,
								VolumeDown, VolumeUp, MediaNext, MediaPrev, MediaStop, MediaPlayPause,
								LaunchMail, LaunchMediaSelect, LaunchApp1, LaunchApp2,
			OEM_1		= 0xBA, OEM_Plus, OEM_Comma, OEM_Minus, OEM_Period, OEM_2, OEM_3,
			OEM_4		= 0xDB, OEM_5, OEM_7, OEM_8,
			OEM_102		= 0xE2,
			ProcessKey	= 0xE5,
			PacketKey	= 0xE7,
			Attn		= 0xF7, CRSEL, EXSEL, EREOF, Play, Zoom, NoName, OEM_Clear,

			MAX_KEYCODE,
			MIN_MOUSE = LeftMouse,
			MAX_MOUSE = (XButton2 + 1),
		};
#else
		enum : int32_t
		{
			MAX_KEYCODE
		};
#endif
	};
}

#endif // !_NATIVE_KEY_H_
