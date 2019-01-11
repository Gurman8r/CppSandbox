#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <MemeWindow/Context.h>
#include <MemeWindow/Icon.h>
#include <MemeWindow/Screen.h>
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
			Normal	 = 0x34001,
			Hidden	 = 0x34002,
			Disabled = 0x34003,
		};
		enum Style : uint32_t
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
			Default	= Resizable | Visible | Decorated | Focused | AutoIconify,
		};

	public:
		Window() {}
		virtual ~Window() {}

		bool create(
			const std::string & title,
			const VideoMode & mode,
			const Style & flags,
			const Context & context);

		virtual bool initialize();

		Window & close();
		Window & maximize();
		Window & minimize();
		Window & pollEvents();
		Window & swapBuffers();

		Window & setCursor(CursorMode value);
		Window & setIcons(const std::vector<Icon> & value);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setTitle(const std::string & value);

		bool	isOpen() const;
		float	getTime() const;

		inline const Context &			context()	const { return m_context; }
		inline const VideoMode &		videoMode()	const { return m_mode; }
		inline const Style &			style()		const { return m_style; }
		inline const vec2i &			position()	const { return m_position; }
		inline const vec2u &			size()		const { return m_size; }
		inline const std::string &		title()		const { return m_title; }
		
		inline const uint32_t & width()		const { return size()[0]; }
		inline const uint32_t & height()	const { return size()[1]; }
		inline const float		aspect()	const { return (float)width() / (float)height(); };

	protected:
		Handle			m_handle;
		Context			m_context;
		VideoMode		m_mode;
		Style			m_style;
		CursorMode		m_cursorMode;
		vec2i			m_position;
		vec2u			m_size;
		std::string		m_title;
	};

	inline Window::Style operator|(Window::Style lhs, Window::Style rhs)
	{
		return (Window::Style)((uint32_t)lhs | (uint32_t)rhs);
	}

	inline Window::Style & operator|=(Window::Style & lhs, const Window::Style & rhs)
	{
		return (lhs = (lhs | rhs));
	}

	inline bool operator&(Window::Style lhs, Window::Style rhs)
	{
		return (bool)(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}

}
#endif // !_WINDOW_H_
