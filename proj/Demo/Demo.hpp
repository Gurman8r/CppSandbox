#ifndef _DEMO_HPP_
#define _DEMO_HPP_

#include <MemeAudio/Audio.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/NativeInput.hpp>
#include <MemeCore/FileSystem.hpp>
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
#include <MemeNet/Client.hpp>
#include <MemeNet/Server.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeEditor/Editor.hpp>

#include "DemoEvents.hpp"
#include "Settings.hpp"

// Resources
/* * * * * * * * * * * * * * * * * * * * */
namespace DEMO
{
	enum : int32_t
	{
		// Projection Matrices
		MIN_PROJ = -1,
		P_persp,
		P_ortho,
		MAX_PROJ,

		// View Matrices
		MIN_VIEW = -1,
		V_camera,
		MAX_VIEW,

		// Transforms (Model Matrices)
		MIN_TRANSFORM = -1,
		T_cube,
		T_quad,
		T_sphere32x24,
		MAX_TRANSFORM,

		// Vertex Arrays
		MIN_VAO = -1,
		VAO_cube,
		VAO_quad,
		VAO_example,
		VAO_sphere8x6,
		VAO_sphere32x24,
		VAO_text,
		MAX_VAO,

		// Vertex Buffers
		MIN_VBO = -1,
		VBO_cube,
		VBO_quad,
		VBO_example,
		VBO_sphere8x6,
		VBO_sphere32x24,
		VBO_text,
		MAX_VBO,

		// Index Buffers
		MIN_IBO = -1,
		IBO_cube,
		IBO_quad,
		MAX_IBO,

		// Frame Buffers
		MIN_FBO = -1,
		FBO_scene,
		MAX_FBO,

		// Render Buffers
		MIN_RBO = -1,
		RBO_scene,
		MAX_RBO,

		// Models
		MIN_MODEL = -1,
		MODEL_example,
		MODEL_sphere8x6,
		MODEL_sphere32x24,
		MAX_MODEL,

		// Text
		MIN_TEXT = -1,
		TXT_dynamic,
		TXT_static,
		MAX_TEXT,
	};
}

// Demo
/* * * * * * * * * * * * * * * * * * * * */
namespace DEMO
{
	class Demo final 
		: public ml::RenderWindow
	{
	public:
		Demo();
		~Demo();

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		void onEvent(const ml::Event * value) override;

		inline const ml::Debug::Status checkStatus()
		{
			static ml::Debug::Status temp;
			temp = m_status;
			m_status = ml::Debug::Success;
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		bool	setupInterpreter();
		
		bool	loadAudio();
		bool	loadBuffers();
		bool	loadFonts();
		bool	loadImages();
		bool	loadTextures();
		bool	loadShaders();
		bool	loadMeshes();
		bool	loadModels();
		bool	loadNetwork();
		bool	loadSprites();

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		void	onEnter(const EnterEvent & ev);
		void	onLoad(const LoadEvent & ev);
		void	onStart(const StartEvent & ev);
		void	onUpdate(const UpdateEvent & ev);
		void	onDraw(const DrawEvent & ev);
		void	onGui(const GuiEvent & ev);
		void	onExit(const ExitEvent & ev);

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		ml::Transform	proj		[MAX_PROJ];
		ml::Transform	view		[MAX_VIEW];
		ml::Transform	transform	[MAX_TRANSFORM];
		ml::VAO			vao			[MAX_VAO];
		ml::VBO			vbo			[MAX_VBO];
		ml::IBO			ibo			[MAX_IBO];
		ml::FBO			fbo			[MAX_FBO];
		ml::RBO			rbo			[MAX_RBO];
		ml::Model		models		[MAX_MODEL];
		ml::Text		text		[MAX_TEXT];

		/* * * * * * * * * * * * * * * * * * * * */

		ml::Debug::Status m_status;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::Texture m_framebuffer;
		ml::vec4f	m_clearColor	= ml::Color::Violet;
		int32_t		m_fboMode		= 0;
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
