#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <MemeWindow/ContextSettings.h>
#include <MemeWindow/VideoMode.h>
#include <MemeCore/Vector4.h>

namespace ml
{
	class ML_WINDOW_API Window
		: public ITrackable
	{
	public:
		using Handle = void *;

	public:
		enum CursorMode : uint32_t
		{
			Normal	 = 0x00034001,
			Hidden	 = 0x00034002,
			Disabled = 0x00034003,
		};
		enum Flags : uint32_t
		{
			None		= (0 << 0),
			Resizable	= (1 << 0),
			Visible		= (1 << 1),
			Decorated	= (1 << 2),
			Focused		= (1 << 3),
			AutoIconify	= (1 << 4),
			Floating	= (1 << 5),
			Maximized	= (1 << 6),

			// Resizable | Visible | Decorated | Focused | AutoIconify
			Default		= Resizable | Visible | Decorated | Focused | AutoIconify,
		};

	public:
		Window() {}
		virtual ~Window() {}

		bool create(
			const std::string & title,
			const VideoMode & mode,
			const Flags & flags,
			const ContextSettings & settings);

		virtual bool initialize();

		Window & close();
		Window & maximize();
		Window & minimize();
		Window & pollEvents();
		Window & setCentered();
		Window & swapBuffers();

		Window & cursorMode(CursorMode value);
		Window & position(const vec2i & value);
		Window & size(const vec2u & value);
		Window & title(const std::string & value);

		bool isOpen() const;

		inline const Handle				handle()		const { return m_handle; }
		inline const ContextSettings &	settings()	const { return m_settings; }
		inline const VideoMode &		videoMode()		const { return m_mode; }
		inline const Flags &			flags()		const { return m_flags; }
		inline const vec2i &			position()	const { return m_position; }
		inline const vec2u &			size()		const { return m_size; }
		inline const std::string &		title()		const { return m_title; }

		inline float getAspect() const { return (float)size()[0] / (float)size()[1]; };

	protected:
		Handle			m_handle;
		ContextSettings m_settings;
		VideoMode		m_mode;
		Flags			m_flags;
		vec2i			m_position;
		vec2u			m_size;
		std::string		m_title;
	};

	inline Window::Flags operator|(Window::Flags lhs, Window::Flags rhs)
	{
		return (Window::Flags)((uint32_t)lhs | (uint32_t)rhs);
	}

	inline Window::Flags & operator|=(Window::Flags & lhs, const Window::Flags & rhs)
	{
		return (lhs = (lhs | rhs));
	}

	inline bool operator&(Window::Flags lhs, Window::Flags rhs)
	{
		return (bool)(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}

}
#endif // !_WINDOW_H_
