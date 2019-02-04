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
			EV_WindowClosed,
			EV_WindowFocused,
			EV_WindowCharTyped,
			EV_WindowMouseScroll,
			EV_WindowMouseMoved,
			EV_WindowMouseEnter,
			EV_WindowMouseButton,
			EV_WindowKey,
			EV_WindowFramebufferResized,

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
			out << "[" << get_type().name() << "] " << width << " " << height;
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
			out << "[" << get_type().name() << "] " << x << " " << y;
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
			out << "[" << get_type().name() << "] ";
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
			out << "[" << get_type().name() << "] " << value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowCharTypedEvent final : public WindowEvent
	{
		uint32_t value;

		WindowCharTypedEvent(uint32_t value)
			: WindowEvent(EV_WindowCharTyped)
			, value(value)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name() << "] " << (char)value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMouseScrollEvent final : public WindowEvent
	{
		double x;
		double y;

		WindowMouseScrollEvent(double x, double y)
			: WindowEvent(EV_WindowMouseScroll)
			, x(x)
			, y(y)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name() << "] " << x << " " << y;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowMouseMoveEvent final : public WindowEvent
	{
		double x;
		double y;

		WindowMouseMoveEvent(double x, double y)
			: WindowEvent(EV_WindowMouseMoved)
			, x(x)
			, y(y)
		{
		}
		
		void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name() << "] " << x << " " << y;
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
			out << "[" << get_type().name() << "] " << value;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowMouseButtonEvent : public WindowEvent
	{
		int32_t button, action, mods;

		WindowMouseButtonEvent(int32_t button, int32_t action, int32_t mods)
			: WindowEvent(EV_WindowMouseButton)
			, button(button)
			, action(action)
			, mods(mods)
		{

		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowKeyEvent : public WindowEvent
	{
		int32_t button, scan, action, mods;

		WindowKeyEvent(int32_t button, int32_t scan, int32_t action, int32_t mods)
			: WindowEvent(EV_WindowKey)
			, button(button)
			, scan(scan)
			, action(action)
			, mods(mods)
		{

		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API WindowFramebufferResizedEvent final : public WindowEvent
	{
		int32_t width;
		int32_t height;

		WindowFramebufferResizedEvent(int32_t width, int32_t height)
			: WindowEvent(EV_WindowFramebufferResized)
			, width(width)
			, height(height)
		{

		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_WINDOW_EVENTS_HPP_
