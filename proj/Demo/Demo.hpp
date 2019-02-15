#ifndef _DEMO_HPP_
#define _DEMO_HPP_

#include <MemeAudio/Audio.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/NativeInput.hpp>
#include <MemeCore/FileSystem.hpp> 
#include <MemeCore/Random.hpp>
#include <MemeCore/Thread.hpp>
#include <MemeEditor/Editor.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/Shapes.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>
#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Model.hpp>
#include <MemeGraphics/Camera.hpp>
#include <MemeNet/Client.hpp>
#include <MemeNet/Server.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeEditor/Editor.hpp>

#include "DemoEvents.hpp"
#include "Settings.hpp"

// Demo
/* * * * * * * * * * * * * * * * * * * * */
namespace DEMO
{
	class Demo final 
		: public ml::RenderWindow
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Demo();
		~Demo();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEvent(const ml::Event * value) override;

		inline const int32_t getError() const
		{
			int32_t temp = 1;
			std::swap((int32_t &)m_error, temp);
			return temp;
		}


	private:
		/* * * * * * * * * * * * * * * * * * * * */

		bool	loadAudio();
		bool	loadBuffers();
		bool	loadFonts();
		bool	loadImages();
		bool	loadInterpreter();
		bool	loadModels();
		bool	loadShaders();
		bool	loadSprites();
		bool	loadTextures();
		bool	loadNetwork();

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		void	onEnter(const EnterEvent & ev);
		void	onLoad(const LoadEvent & ev);
		void	onStart(const StartEvent & ev);
		void	onUpdate(const UpdateEvent & ev);
		void	onDraw(const DrawEvent & ev);
		void	onGui(const GuiEvent & ev);
		void	onExit(const ExitEvent & ev);

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		mutable ml::Debug::Status m_error;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::Thread * m_thread;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::FBO m_frameBuffer;
		ml::RBO m_renderBuffer;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::Transform	m_persp;
		ml::Transform	m_ortho;
		ml::Camera		m_camera;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::VAO m_vaoText;
		ml::VBO m_vboText;

		ml::Text m_textDynamic;
		ml::Text m_textStatic;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::vec3f	m_camPos;
		ml::vec4f	m_clearColor	= ml::Color::Violet;
		int32_t		m_fboMode		= 3;
		int32_t		m_lineMode		= 1;
		ml::vec4f	m_lineColor		= ml::Color::Red;
		float		m_lineDelta		= 1.f;
		int32_t		m_lineSamples	= 16;
		bool		m_animate		= true;

		ml::List<ml::Uniform> m_uniforms;
		size_t m_selected = 0;

		/* * * * * * * * * * * * * * * * * * * * */

		bool show_ml_editor		= false;
		bool show_ml_console	= false;
		bool show_ml_network	= false;
		bool show_imgui_demo	= false;
		bool show_imgui_metrics	= false;
		bool show_imgui_style	= false;
		bool show_imgui_about	= false;

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_DEMO_HPP_
