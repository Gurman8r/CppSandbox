#include <MemeWindow/Window.h>
#include <MemeWindow/Screen.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
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
			
			glfwWindowHint(GLFW_OPENGL_PROFILE, settings.profile);
			glfwWindowHint(GLFW_DEPTH_BITS,		settings.depthBits);
			glfwWindowHint(GLFW_STENCIL_BITS,	settings.stencilBits);
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

						return initialize();
					}
					return ErrorCode::ER_Invalid_Handle;
				}
				return ErrorCode::ER_GLEW_Init_Failure;
			}
			return ErrorCode::ER_GLFW_Create_Failure;
		}
		return ErrorCode::ER_GLFW_Init_Failure;
	}

	Window::ErrorCode Window::initialize()
	{
		// Retrieve the context version number
		int majorVersion = 0;
		int minorVersion = 0;

		// Try the new way first
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		if (glGetError() != GL_INVALID_ENUM)
		{
			m_settings.majorVersion = static_cast<unsigned int>(majorVersion);
			m_settings.minorVersion = static_cast<unsigned int>(minorVersion);
		}
		else
		{
			// Try the old way
			const GLubyte* version = glGetString(GL_VERSION);
			if (version)
			{
				// The beginning of the returned std::string is "major.minor" (this is standard)
				m_settings.majorVersion = version[0] - '0';
				m_settings.minorVersion = version[2] - '0';
			}
			else
			{
				// Can't get the version number, assume 1.1
				m_settings.majorVersion = 1;
				m_settings.minorVersion = 1;
			}
		}

		if (m_settings.majorVersion >= 3)
		{
			// Retrieve the context flags
			int flags = 0;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				m_settings.profile |= ContextSettings::Compat;
			}

			if ((m_settings.majorVersion == 3) && (m_settings.minorVersion == 1))
			{
				m_settings.profile |= ContextSettings::Core;
				int numExtensions = 0;
				glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
				for (uint32_t i = 0; i < static_cast<uint32_t>(numExtensions); ++i)
				{
					const char* extensionString = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));

					if (std::strstr(extensionString, "GL_ARB_compatibility"))
					{
						m_settings.profile &= ~static_cast<uint32_t>(ContextSettings::Core);
						break;
					}
				}
			}
			else if ((m_settings.majorVersion > 3) || (m_settings.minorVersion >= 2))
			{
				// Retrieve the context profile
				int profile = 0;
				glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
				if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
				{
					m_settings.profile |= ContextSettings::Core;
				}
			}
		}

		if (m_settings.multisample)
		{
			glEnable(GL_MULTISAMPLE);
		}

		if (m_settings.sRgbCapable)
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
			if (glIsEnabled(GL_FRAMEBUFFER_SRGB) == GL_FALSE)
			{
				m_settings.sRgbCapable = false;
				return ErrorCode::ER_SRGB_Failure;
			}
		}

		return ErrorCode::ER_Success;
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
		return setPosition((Screen::size() - getSize()) / 2);
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