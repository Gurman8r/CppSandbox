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
		enum ErrorCode : uint32_t
		{
			ER_Success = 0,
			ER_Invalid_Mode,
			ER_GLFW_Init_Failure,
			ER_GLFW_Create_Failure,
			ER_GLEW_Init_Failure,
			ER_Invalid_Handle,
			ER_SRGB_Failure,
		};
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

		ErrorCode create(
			const std::string & title,
			const VideoMode & mode,
			const Flags & flags,
			const ContextSettings & settings);

		ErrorCode initialize();

		Window & clear(const vec4f & value = vec4f::Zero);
		Window & close();
		Window & maximize();
		Window & minimize();
		Window & pollEvents();
		Window & setCentered();
		Window & setCursorMode(CursorMode value);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setTitle(const std::string & value);
		Window & setViewport(const vec2i & pos, const vec2u & size);
		Window & swapBuffers();

		bool isOpen() const;

		inline const Handle				getHandle()		const { return m_handle; }
		inline const ContextSettings &	getSettings()	const { return m_settings; }
		inline const VideoMode &		getMode()		const { return m_mode; }
		inline const Flags &			getFlags()		const { return m_flags; }
		inline const vec2i &			getPosition()	const { return m_position; }
		inline const vec2u &			getSize()		const { return m_size; }
		inline const std::string &		getTitle()		const { return m_title; }

	private:
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
