#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/Debug.h>
#include <MemeWindow/WindowEvents.h>

namespace ml
{
	bool RenderWindow::initialize()
	{
		if (Window::initialize() && OpenGL::init(true))
		{
			// Validate GL Version
			OpenGL::validate(m_context.majorVersion, m_context.minorVersion);

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

			if (m_context.srgbCapable)
			{
				if (!OpenGL::enable(GL::FramebufferSRGB, true))
				{
					ml::Debug::LogWarning("Failed to enable Framebuffer SRGB");
					m_context.srgbCapable = false;
				}
			}

			return true;
		}
		return ml::Debug::LogError("Failed to Initialize GLEW");
	}

	void RenderWindow::onEvent(const Event * value)
	{
		Window::onEvent(value);

		switch (value->eventID())
		{
		case WindowEvent::EV_WindowResized:
			if(auto ev = value->As<WindowResizedEvent>())
			{
				setViewport({ 0, 0 }, { ev->width, ev->height });
			}
		break;
		}
	}
}