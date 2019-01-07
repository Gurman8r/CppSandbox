#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <MemeGraphics/Enum.h>

namespace ml
{
	bool RenderWindow::initialize()
	{
		if (OpenGL::initGL())
		{
			enableFlag(Enum::CullFace);
			enableFlag(Enum::DepthTest);
			enableFlag(Enum::Blend);
			enableFlag(Enum::AlphaTest);
			enableFlag(Enum::Texture2D);

			if (m_settings.multisample)
			{
				enableFlag(Enum::MultiSample);
			}

			if (m_settings.sRgbCapable)
			{
				enableFlag(Enum::FrameBufferSRGB);

				if (!isEnabled(Enum::FrameBufferSRGB))
				{
					ml::Debug::LogWarning("Failed to enableFlag FrameBufferSRGB");
					m_settings.sRgbCapable = false;
				}
			}

			glCheck(glCullFace(Enum::Back));
			glCheck(glDepthFunc(Enum::Less));
			glCheck(glBlendFunc(Enum::SourceAlpha, Enum::OneMinusSourceAlpha));
			glCheck(glAlphaFunc(Enum::Greater, 0.01f));
			glCheck(glActiveTexture(GL_TEXTURE0));


			// Retrieve the context version number
			int majorVersion = 0;
			int minorVersion = 0;

			// Try the new way first
			glCheck(glGetIntegerv(GL_MAJOR_VERSION, &majorVersion));
			glCheck(glGetIntegerv(GL_MINOR_VERSION, &minorVersion));

			if (glGetError() != GL_INVALID_ENUM)
			{
				m_settings.majorVersion = static_cast<unsigned int>(majorVersion);
				m_settings.minorVersion = static_cast<unsigned int>(minorVersion);

				Debug::LogInfo("Using OpenGL Version: {0}.{1}", m_settings.majorVersion, m_settings.minorVersion);
			}
			else
			{
				// Try the old way
				if (const char * version = OpenGL::getVersion())
				{
					m_settings.majorVersion = version[0] - '0';
					m_settings.minorVersion = version[2] - '0';
					Debug::LogInfo("Using OpenGL Version: {0}.{1}", m_settings.majorVersion, m_settings.minorVersion);
				}
				else
				{
					Debug::LogWarning("Can't get the version number, assuming 1.1");
					m_settings.majorVersion = 1;
					m_settings.minorVersion = 1;
				}
			}

			if (m_settings.majorVersion >= 3)
			{
				// Retrieve the context flags
				int flags = 0;
				glCheck(glGetIntegerv(GL_CONTEXT_FLAGS, &flags));

				if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
				{
					Debug::LogWarning("Set Compat Profile");
					m_settings.profile |= ContextSettings::Compat;
				}

				if ((m_settings.majorVersion == 3) && (m_settings.minorVersion == 1))
				{
					m_settings.profile |= ContextSettings::Core;
					int numExtensions = 0;
					glCheck(glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions));
					for (uint32_t i = 0; i < static_cast<uint32_t>(numExtensions); ++i)
					{
						const char* extensionString = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));

						if (std::strstr(extensionString, "GL_ARB_compatibility"))
						{
							Debug::LogWarning("Clear Core Profile");
							m_settings.profile &= ~static_cast<uint32_t>(ContextSettings::Core);
							break;
						}
					}
				}
				else if ((m_settings.majorVersion > 3) || (m_settings.minorVersion >= 2))
				{
					// Retrieve the context profile
					int profile = 0;
					glCheck(glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile));
					if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
					{
						Debug::LogWarning("Set Core Profile");
						m_settings.profile |= ContextSettings::Core;
					}
				}
			}

			return true;
		}
		return ml::Debug::LogError("Failed to Initialize GLEW");
	}

	RenderWindow & RenderWindow::setViewport(const vec2i & pos, const vec2u & size)
	{
		glCheck(glViewport(pos[0], pos[1], size[0], size[1]));
		return (*this);
	}
}