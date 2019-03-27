#ifndef _DEMO_HPP_
#define _DEMO_HPP_

#include <MemeAudio/Audio.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/FileSystem.hpp> 
#include <MemeCore/Random.hpp>
#include <MemeCore/Thread.hpp>
#include <MemeCore/Plugin.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/Canvas.hpp>
#include <MemeGraphics/Effect.hpp>
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
#include <MemeScript/Interpreter.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Particle.hpp>

#include "DemoEvents.hpp"
#include "DemoSettings.hpp"

// Demo
/* * * * * * * * * * * * * * * * * * * * */
namespace DEMO
{
	class Demo final : public ml::RenderWindow
	{
	public:
		Demo();
		~Demo();

	public:
		void onEvent(const ml::IEvent * value) override;

	private:
		bool	loadResources();
		bool	loadBuffers();
		bool	loadNetwork();

	private:
		void	onEnter(const EnterEvent & ev);
		void	onLoad(const LoadEvent & ev);
		void	onStart(const StartEvent & ev);
		void	onFixedUpdate(const FixedUpdateEvent & ev);
		void	onUpdate(const UpdateEvent & ev);
		void	onDraw(const DrawEvent & ev);
		void	onGui(const GuiEvent & ev);
		void	onExit(const ExitEvent & ev);

	private:
		bool ML_MainMenuBar_draw();
		bool ML_Dockspace_draw(bool * p_open);
		bool ML_Inspector_draw(bool * p_open);
		bool ML_SceneView_draw(bool * p_open);
		bool ML_DemoWindow_draw(bool * p_open);

	private:
		// Sprites/2D
		/* * * * * * * * * * * * * * * * * * * * */

		ml::VAO m_batch_vao;
		ml::VBO m_batch_vbo;

		using TextMap	= ml::HashMap<ml::String, ml::Text>;
		using EffectMap = ml::HashMap<ml::String, ml::Effect>;

		TextMap		m_txtMap;
		EffectMap	m_fboMap;

		// Uniforms
		/* * * * * * * * * * * * * * * * * * * * */
		struct DemoUniforms
		{
			// Matrices
			ml::Transform	persp;
			ml::Transform	ortho;
			ml::Transform	camera;

			bool			camAnimate	= true;
			ml::vec3f		camPos		= { 0, 1, 10 };
			float			camSpd		= 1.f;
		
			ml::vec3f		lightPos		= { 0, 1, 30 };
			ml::vec4f		lightCol		= ml::Color::LightYellow;
			float			ambient		= 0.01f;
			float			specular		= 0.5f;
			int32_t			shininess		= 8;

			ml::vec4f		clearColor	= ml::Color::Gray;
			int32_t			effectMode		= 0;

			int32_t			lineMode		= 1;
			ml::vec4f		lineColor		= ml::Color::Red;
			float			lineDelta		= 1.f;
			float			lineSize		= 0.5f;
			int32_t			lineSamples	= 16;
			bool			animate		= true;
		};
		DemoUniforms uni;

		// GUI
		/* * * * * * * * * * * * * * * * * * * * */
		struct DemoGUI
		{
			/* * * * * * * * * * * * * * * * * * * * */
			bool show_imgui_demo	= false;
			bool show_imgui_metrics	= false;
			bool show_imgui_style	= false;
			bool show_imgui_about	= false;
			/* * * * * * * * * * * * * * * * * * * * */
			bool show_dockspace		= true;
			bool show_terminal		= true;
			bool show_browser		= true;
			bool show_builder		= true;
			bool show_inspector		= true;
			bool show_scene			= true;
			bool show_texteditor	= true;
			bool show_hierarchy		= true;
			bool show_resources		= true;
			bool show_network		= true;
			bool show_demowindow	= true;
			/* * * * * * * * * * * * * * * * * * * * */
		};
		DemoGUI gui;

		// Physics
		/* * * * * * * * * * * * * * * * * * * * */

		struct DemoPhysics
		{
			ml::Rigidbody	rigidbody;
			ml::Particle	particle;
		};
		DemoPhysics phys;

		// Testing
		/* * * * * * * * * * * * * * * * * * * * */

		ml::Thread * m_thread;

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_DEMO_HPP_
