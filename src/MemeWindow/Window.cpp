#include <MemeWindow/Window.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma comment (lib, "glew32s.lib") // Link to glew32s.lib

namespace ml
{
	Window::Window()
	{
	}

	Window::~Window()
	{
	}

	int Window::create(const std::string & title, const VideoMode & mode, const Style & style, const ContextSettings & settings)
	{
		m_settings = settings;
		m_mode = mode;

		if (glfwInit() == GL_TRUE)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.minorVersion);
			glfwWindowHint(GLFW_OPENGL_PROFILE, settings.attributeFlags);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			GLFWwindow* window;
			if (window = glfwCreateWindow(mode.width, mode.height, title.c_str(), NULL, NULL))
			{
				m_ptr = static_cast<GLFWwindow*>(window);

				glfwMakeContextCurrent(window);

				glewExperimental = GL_TRUE;

				if (glewInit() == GLEW_OK)
				{
					glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h)
					{
					});

					glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t c)
					{
					});

					glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y)
					{
					});

					// Centered
					VideoMode desktop = VideoMode::getDesktopMode();
					setPosition(vec2u(
						(desktop.width - mode.width),
						(desktop.height - mode.height)) / 2);

					return 0;
				}
				return 2;
			}
			return 1;
		}
		return -1;
		
	}

	Window & Window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		return (*this);
	}

	Window & Window::pollEvents()
	{
		glfwPollEvents();
		return (*this);
	}

	Window & Window::setPosition(const vec2u & value)
	{
		glfwSetWindowPos(static_cast<GLFWwindow*>(m_ptr), value[0], value[1]);
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_ptr));
		return (*this);
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_ptr));
	}

	
}