#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeWindow/WindowEvents.hpp>

namespace ml
{
	bool RenderWindow::setup()
	{
		return setup(true);
	}

	bool RenderWindow::setup(bool experimental)
	{
		if (Window::setup() && OpenGL::init(experimental))
		{
			// Validate GL Version
			OpenGL::validate(m_context.majorVersion, m_context.minorVersion);

			// Setup GL
			OpenGL::enable(GL::CullFace);
			OpenGL::enable(GL::DepthTest);
			OpenGL::enable(GL::Blend);
			OpenGL::enable(GL::AlphaTest);
			OpenGL::enable(GL::Texture2D);

			OpenGL::cullFace(GL::Back);
			OpenGL::depthFunc(GL::Less);
			OpenGL::blendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);
			OpenGL::alphaFunc(GL::Greater, 0.01f);
			OpenGL::activeTexture(GL::Texture0);

			if (m_context.multisample)
			{
				OpenGL::enable(GL::Multisample);
			}

			if (m_context.srgbCapable)
			{
				if (!OpenGL::enable(GL::FramebufferSRGB, true))
				{
					ml::Debug::logWarning("Failed to enable Framebuffer SRGB");
					m_context.srgbCapable = false;
				}
			}

			return true;
		}
		return ml::Debug::logError("Failed to Initialize GLEW");
	}

	void RenderWindow::onEvent(const Event * value)
	{
		Window::onEvent(value);

		switch (value->eventID())
		{
		case WindowEvent::EV_WindowSize:
			if(auto ev = value->Cast<WindowSizeEvent>())
			{
				setViewport({ 0, 0 }, getFramebufferSize());
			}
		break;
		case WindowEvent::EV_FramebufferSize:
			if (auto ev = value->Cast<FramebufferSizeEvent>())
			{
			}
			break;
		}
	}
}