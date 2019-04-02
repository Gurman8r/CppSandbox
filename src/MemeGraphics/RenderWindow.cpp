#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeWindow/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderWindow::RenderWindow()
		: Window()
	{
	}
	RenderWindow::~RenderWindow() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool RenderWindow::setup()
	{
		return setup(true);
	}

	bool RenderWindow::setup(bool experimental)
	{
		if (Window::setup() && ML_GL.init(experimental))
		{
			// Validate GL Version
			ML_GL.validate(m_context.majorVersion, m_context.minorVersion);

			// Setup GL
			ML_GL.enable(GL::CullFace);
			ML_GL.enable(GL::DepthTest);
			ML_GL.enable(GL::Blend);
			ML_GL.enable(GL::AlphaTest);
			ML_GL.enable(GL::Texture2D);

			ML_GL.cullFace(GL::Back);
			ML_GL.depthFunc(GL::Less);
			ML_GL.blendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);
			ML_GL.alphaFunc(GL::Greater, 0.01f);

			ML_GL.activeTexture(GL::Texture0);

			if (m_context.multisample)
			{
				ML_GL.enable(GL::Multisample);
			}

			if (m_context.srgbCapable)
			{
				if (!ML_GL.enable(GL::FramebufferSRGB))
				{
					ml::Debug::logWarning("Failed to enable Framebuffer SRGB");
					m_context.srgbCapable = false;
				}
			}

			return true;
		}
		return ml::Debug::logError("Failed to Initialize GLEW");
	}

	void RenderWindow::onEvent(const IEvent * value)
	{
		Window::onEvent(value);

		switch (*value)
		{
		case WindowEvent::EV_WindowSize:
			if(auto ev = value->as<WindowSizeEvent>())
			{
			}
		break;
		case WindowEvent::EV_FramebufferSize:
			if (auto ev = value->as<FramebufferSizeEvent>())
			{
				setViewport({ 0, 0 }, getFramebufferSize());
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}