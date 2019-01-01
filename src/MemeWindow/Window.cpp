#include <MemeWindow/Window.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment (lib, "glew32s.lib") // Link to glew32s.lib

namespace ml
{
	Window::ErrorCode Window::create(
		const std::string & title, 
		const VideoMode & mode, 
		const Flags & flags,
		const ContextSettings & settings)
	{
		m_handle	= NULL;
		m_title		= title;
		m_settings	= settings;
		m_mode		= mode;
		m_flags		= flags;
		m_position	= vec2i::Zero;
		m_size		= getMode().size();

		if (!getMode().isValid())
		{
			return ErrorCode::ER_Invalid_Mode;
		}

		if (glfwInit() == GL_TRUE)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.minorVersion);
			
			glfwWindowHint(GLFW_DEPTH_BITS,		settings.depthBits);
			glfwWindowHint(GLFW_STENCIL_BITS,	settings.stencilBits);
			glfwWindowHint(GLFW_OPENGL_PROFILE, settings.profile);
			glfwWindowHint(GLFW_SRGB_CAPABLE,	settings.sRgbCapable);

			glfwWindowHint(GLFW_RESIZABLE,		(flags & Flags::Resizable));
			glfwWindowHint(GLFW_VISIBLE,		(flags & Flags::Visible));
			glfwWindowHint(GLFW_DECORATED,		(flags & Flags::Decorated));
			glfwWindowHint(GLFW_FOCUSED,		(flags & Flags::Focused));
			glfwWindowHint(GLFW_AUTO_ICONIFY,	(flags & Flags::AutoIconify));
			glfwWindowHint(GLFW_FLOATING,		(flags & Flags::Floating));
			glfwWindowHint(GLFW_MAXIMIZED,		(flags & Flags::Maximized));

			if (GLFWwindow* ptr = glfwCreateWindow(
				getMode().width,	// width
				getMode().height,	// height
				title.c_str(),		// title
				NULL,				// monitor
				NULL))				// share
			{
				glfwMakeContextCurrent(ptr);

				glewExperimental = GL_TRUE;

				if (glewInit() == GLEW_OK)
				{
					if (m_handle = static_cast<GLFWwindow*>(ptr))
					{
						glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_handle),
							[](GLFWwindow* window, int w, int h)
						{
						});

						glfwSetWindowPosCallback(static_cast<GLFWwindow*>(m_handle),
							[](GLFWwindow* window, int w, int h)
						{
						});

						glfwSetCharCallback(static_cast<GLFWwindow*>(m_handle),
							[](GLFWwindow* window, uint32_t c)
						{
						});

						glfwSetScrollCallback(static_cast<GLFWwindow*>(m_handle),
							[](GLFWwindow* window, double x, double y)
						{
						});

						glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_handle),
							[](GLFWwindow* window)
						{
						});

						glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(m_handle),
							[](GLFWwindow* window, int width, int height)
						{
						});

						glfwSetWindowFocusCallback(static_cast<GLFWwindow*>(m_handle),
							[](GLFWwindow* window, int focused) 
						{
						});

						glEnable(GL_CULL_FACE);
						glCullFace(GL_BACK);

						glEnable(GL_DEPTH_TEST);
						glDepthFunc(GL_LESS);

						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						glEnable(GL_ALPHA_TEST);
						glAlphaFunc(GL_GREATER, 0.01f);

						glEnable(GL_TEXTURE_2D);
						glActiveTexture(GL_TEXTURE0);

						if (settings.multisample)
							glEnable(GL_MULTISAMPLE);

						if (settings.sRgbCapable)
							glEnable(GL_FRAMEBUFFER_SRGB);

						return ErrorCode::ER_Success;
					}
					return ErrorCode::ER_Invalid_Handle;
				}
				return ErrorCode::ER_GLEW_Init_Failure;
			}
			return ErrorCode::ER_GLFW_Create_Failure;
		}
		return ErrorCode::ER_GLFW_Init_Failure;
	}


	Window & Window::clear(const vec4f & value)
	{
		glClearColor(value[0], value[1], value[2], value[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return (*this);
	}

	Window & Window::close()
	{
		glfwSetWindowShouldClose(static_cast<GLFWwindow*>(m_handle), GLFW_TRUE);
		return (*this);
	}

	Window & Window::maximize()
	{
		glfwRestoreWindow(static_cast<GLFWwindow*>(m_handle));
		return (*this);
	}

	Window & Window::minimize()
	{
		glfwIconifyWindow(static_cast<GLFWwindow*>(m_handle));
		return (*this);
	}

	Window & Window::pollEvents()
	{
		glfwPollEvents();
		return (*this);
	}

	Window & Window::setCentered()
	{
		return setPosition((VideoMode::getDesktopMode().size() - getMode().size()) / 2);
	}

	Window & Window::setCursorMode(CursorMode value)
	{
		glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, value);
		return (*this);
	}

	Window & Window::setPosition(const vec2i & value)
	{
		m_position = value;
		glfwSetWindowPos(static_cast<GLFWwindow*>(m_handle), value[0], value[1]);
		return (*this);
	}

	Window & Window::setSize(const vec2u & value)
	{
		m_size = value;
		glfwSetWindowSize(static_cast<GLFWwindow*>(m_handle), value[0], value[1]);
		return (*this);
	}

	Window & Window::setTitle(const std::string & value)
	{
		m_title = value;
		glfwSetWindowTitle(static_cast<GLFWwindow*>(m_handle), value.c_str());
		return (*this);
	}

	Window & Window::setViewport(const vec2i & pos, const vec2u & size)
	{
		glViewport(pos[0], pos[1], size[0], size[1]);
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
		return (*this);
	}


	bool Window::isOpen() const
	{
		return !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
	}
}