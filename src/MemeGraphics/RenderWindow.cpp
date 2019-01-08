#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	bool RenderWindow::initialize()
	{
		if (OpenGL::initGL(true))
		{
			// Setup GL

			enableFlag(GL::CullFace);
			enableFlag(GL::DepthTest);
			enableFlag(GL::Blend);
			enableFlag(GL::AlphaTest);
			enableFlag(GL::Texture2D);

			if (m_settings.multisample)
			{
				enableFlag(GL::Multisample);
			}

			if (m_settings.sRgbCapable)
			{
				enableFlag(GL::FramebufferSRGB);
				if (!getFlag(GL::FramebufferSRGB))
				{
					ml::Debug::LogWarning("Failed to enable Framebuffer SRGB");
					m_settings.sRgbCapable = false;
				}
			}

			setAlphaFunc(GL::Greater, 0.01f);
			setBlendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);
			setCullFace(GL::Back);
			setDepthFunc(GL::Less);
			setActiveTexture(GL::Texture0);


			// Validate GL Version

			int majorVersion = OpenGL::getInt(GL::MajorVersion);
			int minorVersion = OpenGL::getInt(GL::MinorVersion);

			if (OpenGL::getError() != GL::InvalidEnum)
			{
				m_settings.majorVersion = (uint32_t)(majorVersion);
				m_settings.minorVersion = (uint32_t)(minorVersion);
			}
			else if (const char * version = OpenGL::getString(GL::Version))
			{
				m_settings.majorVersion = version[0] - '0';
				m_settings.minorVersion = version[2] - '0';
			}
			else
			{
				Debug::LogWarning("Can't get the version number, assuming 1.1");
				m_settings.majorVersion = 1;
				m_settings.minorVersion = 1;
			}

			Debug::LogInfo("Using OpenGL Version: {0}.{1}",
				m_settings.majorVersion,
				m_settings.minorVersion);

			if (m_settings.majorVersion >= 3)
			{
				// Retrieve the context flags
				int flags = OpenGL::getInt(GL::ContextFlags);
				if (flags & GL::ContextFlagDebugBit)
				{
					Debug::LogWarning("Set Compat Profile");
					m_settings.profile |= ContextSettings::Compat;
				}

				if ((m_settings.majorVersion == 3) && (m_settings.minorVersion == 1))
				{
					m_settings.profile |= ContextSettings::Core;
					
					int numExtensions = (uint32_t)OpenGL::getInt(GL::NumExtensions);
					for (uint32_t i = 0; i < (uint32_t)(numExtensions); ++i)
					{
						const char* extensions = OpenGL::getString(GL::Extensions, i);
						if (std::strstr(extensions, "GL_ARB_compatibility"))
						{
							Debug::LogWarning("Clear Core Profile");
							m_settings.profile &= ~ContextSettings::Core;
							break;
						}
					}
				}
				else if ((m_settings.majorVersion > 3) || (m_settings.minorVersion >= 2))
				{
					// Retrieve the context profile
					int profile = OpenGL::getInt(GL::ContextProfileMask);
					if (profile & GL::ContextCoreProfileBit)
					{
						Debug::LogWarning("Set Core Profile");
						m_settings.profile |= ContextSettings::Core;
					}
				}
			}
			return ml::Debug::LogInfo("OK");
		}
		return ml::Debug::LogError("Failed to Initialize GLEW");
	}
}