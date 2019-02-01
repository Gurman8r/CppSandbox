#ifndef _WINDOW_EVENTS_HPP_
#define _WINDOW_EVENTS_HPP_

#include <MemeWindow/Export.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowEvent : public Event
	{
		enum : int32_t
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

			MAX_WINDOW_EVENT
		};

		WindowEvent(int32_t eventID)
			: Event(eventID)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowResizedEvent final : public WindowEvent
	{
		int32_t width;
		int32_t height;

		WindowResizedEvent(int32_t width, int32_t height)
			: WindowEvent(EV_WindowResized)
			, width(width)
			, height(height)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << width << " " << height;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMovedEvent final : public WindowEvent
	{
		int32_t x;
		int32_t y;

		WindowMovedEvent(int32_t x, int32_t y)
			: WindowEvent(EV_WindowMoved)
			, x(x)
			, y(y)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << x << " " << y;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowCharEvent final : public WindowEvent
	{
		uint32_t value;

		WindowCharEvent(uint32_t value)
			: WindowEvent(EV_WindowChar)
			, value(value)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << (char)value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowScrollEvent final : public WindowEvent
	{
		double x;
		double y;

		WindowScrollEvent(double x, double y)
			: WindowEvent(EV_WindowScroll)
			, x(x)
			, y(y)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << x << " " << y;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowClosedEvent final : public WindowEvent
	{
		WindowClosedEvent()
			: WindowEvent(EV_WindowClosed)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowFocusedEvent final : public WindowEvent
	{
		int32_t value;

		WindowFocusedEvent(int32_t value)
			: WindowEvent(EV_WindowFocused)
			, value(value)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMouseMoveEvent final : public WindowEvent
	{
		double x;
		double y;

		WindowMouseMoveEvent(double x, double y)
			: WindowEvent(EV_WindowMouseMove)
			, x(x)
			, y(y)
		{
		}
		
		void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << x << " " << y;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMouseEnterEvent final : public WindowEvent
	{
		int32_t value;

		WindowMouseEnterEvent(int32_t value)
			: WindowEvent(EV_WindowMouseEnter)
			, value(value)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_WINDOW_EVENTS_HPP_
