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
			OpenGL::enable(GL::CullFace);
			OpenGL::cullFace(GL::Back);

			OpenGL::enable(GL::DepthTest);
			OpenGL::depthFunc(GL::Less);

			OpenGL::enable(GL::Blend);
			OpenGL::blendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);

			OpenGL::enable(GL::AlphaTest);
			OpenGL::alphaFunc(GL::Greater, 0.01f);

			OpenGL::enable(GL::Texture2D);
			OpenGL::activeTexture(GL::Texture0);

			if (m_context.multisample)
			{
				OpenGL::enable(GL::Multisample);
			}

			if (m_context.sRgbCapable)
			{
				if (!OpenGL::enable(GL::FramebufferSRGB, true))
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