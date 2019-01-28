#ifndef _WINDOW_EVENTS_H_
#define _WINDOW_EVENTS_H_

#include <MemeWindow/Export.h>
#include <MemeCore/EventSystem.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	enum WindowEventID : int32_t
	{
		MIN_WINDOW_EVENT = Event::EV_WINDOW + 1,

		EV_WindowResized,
		EV_WindowMoved,
		EV_WindowChar,
		EV_WindowScroll,
		EV_WindowClosed,
		EV_WindowFocused,
		EV_WindowMouseMove,
		EV_WindowMouseEnter,
		EV_WindowMouseButton,

		MAX_WINDOW_EVENT
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowResizedEvent final : public Event
	{
		int32_t width;
		int32_t height;

		WindowResizedEvent(int32_t width, int32_t height)
			: Event(EV_WindowResized)
			, width(width)
			, height(height)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] " 
				<< width << " " << height;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMovedEvent final : public Event
	{
		int32_t x;
		int32_t y;

		WindowMovedEvent(int32_t x, int32_t y)
			: Event(EV_WindowMoved)
			, x(x)
			, y(y)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] "
				<< x << " " << y;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowCharEvent final : public Event
	{
		uint32_t value;

		WindowCharEvent(uint32_t value)
			: Event(EV_WindowChar)
			, value(value)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] "
				<< (char)value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowScrollEvent final : public Event
	{
		double x;
		double y;

		WindowScrollEvent(double x, double y)
			: Event(EV_WindowScroll)
			, x(x)
			, y(y)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] "
				<< x << " " << y;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowClosedEvent final : public Event
	{
		WindowClosedEvent()
			: Event(EV_WindowClosed)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowFocusedEvent final : public Event
	{
		int32_t value;

		WindowFocusedEvent(int32_t value)
			: Event(EV_WindowFocused)
			, value(value)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] "
				<< (value ? "true" : "false");
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMouseMoveEvent final : public Event
	{
		double x;
		double y;

		WindowMouseMoveEvent(double x, double y)
			: Event(EV_WindowMouseMove)
			, x(x)
			, y(y)
		{
		}
		
		void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] "
				<< x << " " << y;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMouseEnterEvent final : public Event
	{
		int32_t value;

		WindowMouseEnterEvent(int32_t value)
			: Event(EV_WindowMouseEnter)
			, value(value)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] "
				<< (value ? "true" : "false");
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowMouseButtonEvent final : public Event
	{
		int32_t button;
		int32_t action;
		int32_t mods;

		WindowMouseButtonEvent(int32_t button, int32_t action, int32_t mods)
			: Event(EV_WindowMouseButton)
		{
			Debug::Log("EVENT -> {0}", (*this));
		}

		void serialize(std::ostream & out) const override
		{
			out << "[" << eventID() << "]"
				<< "[" << get_type().name() << "] "
				<< button << " " << action << " " << mods;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_WINDOW_EVENTS_H_
