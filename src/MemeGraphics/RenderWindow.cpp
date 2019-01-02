#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	Window::ErrorCode RenderWindow::initialize()
	{
		glewExperimental = GL_TRUE;

		if (glewInit() == GLEW_OK)
		{
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
		return ErrorCode::ER_GLEW_Init_Failure;
	}

	RenderWindow & RenderWindow::setViewport(const vec2i & pos, const vec2u & size)
	{
		glViewport(pos[0], pos[1], size[0], size[1]);
		return (*this);
	}
}