#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	bool RenderWindow::initialize()
	{
		if (Window::initialize() && OpenGL::init(true))
		{
			// Validate GL Version
			Debug::Log("OpenGL version supported by this platform: {0}", 
				OpenGL::getString(GL::Version));

			m_settings.majorVersion = (uint32_t)OpenGL::getInt(GL::MajorVersion);
			m_settings.minorVersion = (uint32_t)OpenGL::getInt(GL::MinorVersion);

			if (OpenGL::getError() == GL::InvalidEnum)
			{
				if (const char * version = OpenGL::getString(GL::Version))
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
			}

			Debug::Log("Using OpenGL Version: {0}.{1}",
				m_settings.majorVersion,
				m_settings.minorVersion);


			// Setup GL
			enable(GL::CullFace);
			cullFace(GL::Back);

			enable(GL::DepthTest);
			depthFunc(GL::Less);

			enable(GL::Blend);
			blendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);

			enable(GL::AlphaTest);
			alphaFunc(GL::Greater, 0.01f);

			enable(GL::Texture2D);
			activeTexture(GL::Texture0);

			if (m_settings.multisample)
			{
				enable(GL::Multisample);
			}

			if (m_settings.sRgbCapable)
			{
				if (!enable(GL::FramebufferSRGB, true))
				{
					ml::Debug::LogWarning("Failed to enable Framebuffer SRGB");
					m_settings.sRgbCapable = false;
				}
			}

			return true;
		}
		return ml::Debug::LogError("Failed to Initialize GLEW");
	}
}