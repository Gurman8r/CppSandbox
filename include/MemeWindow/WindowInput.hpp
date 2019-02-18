#ifndef _WINDOW_INPUT_HPP_
#define _WINDOW_INPUT_HPP_

#include <MemeWindow/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/BitHelper.hpp>

#define ML_RELEASE	0
#define ML_PRESS	1
#define ML_REPEAT	2

#define ML_MOD_SHIFT	(0 << 1)
#define ML_MOD_CTRL		(1 << 1)
#define ML_MOD_ALT		(1 << 2)
#define ML_MOD_SUPER	(1 << 3)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	enum KeyMod : int32_t
	{
		ModShift = ML_MOD_SHIFT,
		ModCtrl	 = ML_MOD_CTRL,
		ModAlt	 = ML_MOD_ALT,
		ModSuper = ML_MOD_SUPER,
	};

	inline KeyMod operator&(const KeyMod l, const KeyMod r)
	{
		return static_cast<KeyMod>((int)l & (int)r);
	}
	inline KeyMod operator|(const KeyMod l, const KeyMod r)
	{
		return static_cast<KeyMod>((int)l | (int)r);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	enum KeyState : int32_t
	{
		Release	= ML_RELEASE, 
		Press	= ML_PRESS, 
		Repeat	= ML_REPEAT,
	};

	/* * * * * * * * * * * * * * * * * * * * */

	enum MouseButton : int32_t
	{
		Button0,
		Button1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,

		MAX_MOUSE_BUTTON,
		MIN_MOUSE_BUTTON = Button0,
	};

	/* * * * * * * * * * * * * * * * * * * * */

	enum KeyCode : int32_t
	{
		Space		= 32,
		Apostrophe	= 39,	/* ' */
		Comma		= 44,	/* , */
		Minus		= 45,	/* - */
		Period		= 46,	/* . */
		Slash		= 47,	/* / */
		Num0		= 48,
		Num1		= 49,
		Num2		= 50,
		Num3		= 51,
		Num4		= 52,
		Num5		= 53,
		Num6		= 54,
		Num7		= 55,
		Num8		= 56,
		Num9		= 57,
		Semicolon	= 59,	/* ; */
		Equal		= 61,	/* = */
		A			= 65,
		B			= 66,
		C			= 67,
		D			= 68,
		E			= 69,
		F			= 70,
		G			= 71,
		H			= 72,
		I			= 73,
		J			= 74,
		K			= 75,
		L			= 76,
		M			= 77,
		N			= 78,
		O			= 79,
		P			= 80,
		Q			= 81,
		R			= 82,
		S			= 83,
		T			= 84,
		U			= 85,
		V			= 86,
		W			= 87,
		X			= 88,
		Y			= 89,
		Z			= 90,
		LeftBracket	= 91,	/* [ */
		Backslash	= 92,	/* \ */
		RightBracket= 93,	/* ] */
		GraveAccent	= 96,	/* ` */
		World_1		= 161,	/* non-US #1 */
		World_2		= 162,	/* non-US #2 */
		Escape		= 256,
		Enter		= 257,
		Tab			= 258,
		Backspace	= 259,
		Insert		= 260,
		Delete		= 261,
		Right		= 262,
		Left		= 263,
		Down		= 264,
		Up			= 265,
		PageUp		= 266,
		PageDown	= 267,
		Home		= 268,
		End			= 269,
		CapsLock	= 280,
		ScrollLock	= 281,
		NumLock		= 282,
		PrintScreen	= 283,
		Pause		= 284,
		F1			= 290,
		F2			= 291,
		F3			= 292,
		F4			= 293,
		F5			= 294,
		F6			= 295,
		F7			= 296,
		F8			= 297,
		F9			= 298,
		F10			= 299,
		F11			= 300,
		F12			= 301,
		F13			= 302,
		F14			= 303,
		F15			= 304,
		F16			= 305,
		F17			= 306,
		F18			= 307,
		F19			= 308,
		F20			= 309,
		F21			= 310,
		F22			= 311,
		F23			= 312,
		F24			= 313,
		F25			= 314,
		NUM_0		= 320,
		NUM_1		= 321,
		NUM_2		= 322,
		NUM_3		= 323,
		NUM_4		= 324,
		NUM_5		= 325,
		NUM_6		= 326,
		NUM_7		= 327,
		NUM_8		= 328,
		NUM_9		= 329,
		NUM_Decimal	= 330,
		NUM_Divide	= 331,
		NUM_Multiply= 332,
		NUM_Subtract= 333,
		NUM_Add		= 334,
		NUM_Enter	= 335,
		NUM_Equal	= 336,
		LeftShift	= 340,
		LeftControl	= 341,
		LeftAlt		= 342,
		LeftSuper	= 343,
		RightShift	= 344,
		RightControl= 345,
		RightAlt	= 346,
		RightSuper	= 347,
		Menu		= 348,
			
		MAX_KEYCODE,
		MIN_KEYCODE = Space,

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_WINDOW_INPUT_HPP_
