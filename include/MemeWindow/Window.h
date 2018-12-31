#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <MemeWindow/ContextSettings.h>
#include <MemeWindow/VideoMode.h>
#include <MemeCore/Enum.h>

namespace ml
{
	class ML_WINDOW_API Window
		: public ITrackable
	{
	public:
		enum CursorMode : int
		{
			Normal	 = 0x00034001,
			Hidden	 = 0x00034002,
			Disabled = 0x00034003,
		};
		enum Style : int
		{
			None		= ML_FLAG_0, // No border / title bar (mutually exclusive)
			Titlebar	= ML_FLAG_1, // Title bar + fixed border
			Resize		= ML_FLAG_2, // Titlebar + resizable border + maximize button
			Close		= ML_FLAG_3, // Titlebar + close button
			Fullscreen	= ML_FLAG_4, // Fullscreen configuration (mutually exclusive)
			Default		= Titlebar | Resize | Close,
		};

	public:
		Window();
		virtual ~Window();

		int create(
			const std::string & title,
			const VideoMode & mode,
			const Style & style,
			const ContextSettings & settings);

		Window & clear();
		Window & pollEvents();
		Window & setPosition(const vec2u & value);
		Window & swapBuffers();

		bool shouldClose() const;

		inline const ContextSettings & settings() const { return m_settings; }
		inline const VideoMode & mode() const { return m_mode; }

	private:
		ContextSettings m_settings;
		VideoMode		m_mode;
		void *			m_ptr;

	};

	inline Window::Window::Style operator|(const Window::Style& lhs, const Window::Style& rhs)
	{
		return static_cast<Window::Style>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	inline Window::Style& operator|=(Window::Style& lhs, const Window::Style rhs)
	{
		return (lhs = (lhs | rhs));
	}
}
#endif // !_WINDOW_H_
