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
			// Validate OpenGL Version
			ML_GL.validateVersion(m_context.majorVersion, m_context.minorVersion);

			// Setup OpenGL
			ML_GL.enable(GL::AlphaTest);
			ML_GL.enable(GL::Blend);
			ML_GL.enable(GL::CullFace);
			ML_GL.enable(GL::DepthTest);
			ML_GL.enable(GL::Multisample, m_context.multisample);
			ML_GL.enable(GL::Texture2D);

			if (!ML_GL.enable(GL::FramebufferSRGB, m_context.srgbCapable))
			{
				ml::Debug::logWarning("Failed to enable Framebuffer SRGB");
				m_context.srgbCapable = false;
			}
			
			ML_GL.cullFace(GL::Back);
			ML_GL.depthFunc(GL::Less);
			ML_GL.blendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);
			ML_GL.alphaFunc(GL::Greater, 0.01f);
			ML_GL.activeTexture(GL::Texture0);

			return true;
		}
		return ml::Debug::logError("Failed to Initialize GLEW");
	}

	void RenderWindow::onEvent(const IEvent * value)
	{
		Window::onEvent(value);

		switch (*value)
		{
		case WindowEvent::EV_FrameSize:
			if (const auto * ev = value->as<FrameSizeEvent>())
			{
				this->setViewport(vec2f::Zero, ev->size());
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}