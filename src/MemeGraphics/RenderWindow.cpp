#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	void RenderWindow::validate()
	{
		if (OpenGL::good())
		{
			Debug::Log("OpenGL version supported by this platform: {0}",
				OpenGL::getString(GL::Version));

			m_context.majorVersion = (uint32_t)OpenGL::getInt(GL::MajorVersion);
			m_context.minorVersion = (uint32_t)OpenGL::getInt(GL::MinorVersion);

			if (OpenGL::getError() == GL::InvalidEnum)
			{
				if (const char * version = OpenGL::getString(GL::Version))
				{
					m_context.majorVersion = version[0] - '0';
					m_context.minorVersion = version[2] - '0';
				}
				else
				{
					Debug::LogWarning("Can't get the version number, assuming 1.1");
					m_context.majorVersion = 1;
					m_context.minorVersion = 1;
				}
			}

			Debug::Log("Using OpenGL Version: {0}.{1}",
				m_context.majorVersion,
				m_context.minorVersion);
		}
	}

	bool RenderWindow::initialize()
	{
		if (Window::initialize() && OpenGL::init(true))
		{
			// Validate GL Version
			validate();

			// Setup GL
			setEnabled(GL::CullFace);
			setCullFace(GL::Back);

			setEnabled(GL::DepthTest);
			setDepthFunc(GL::Less);

			setEnabled(GL::Blend);
			setBlendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);

			setEnabled(GL::AlphaTest);
			setAlphaFunc(GL::Greater, 0.01f);

			setEnabled(GL::Texture2D);
			setActiveTexture(GL::Texture0);

			if (m_context.multisample)
			{
				setEnabled(GL::Multisample);
			}

			if (m_context.sRgbCapable)
			{
				if (!setEnabled(GL::FramebufferSRGB, true))
				{
					ml::Debug::LogWarning("Failed to enable Framebuffer SRGB");
					m_context.sRgbCapable = false;
				}
			}

			return true;
		}
		return ml::Debug::LogError("Failed to Initialize GLEW");
	}
}