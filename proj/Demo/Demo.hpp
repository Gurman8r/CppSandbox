#ifndef _DEMO_HPP_
#define _DEMO_HPP_

#include "DemoEvents.hpp"
#include "DemoSettings.hpp"
#include <MemeCore/Thread.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Particle.hpp>

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

	public:
		void onEvent(const ml::IEvent * value) override;

	private:
		void	onEnter(const EnterEvent & ev);
		void	onLoad(const LoadEvent & ev);
		void	onStart(const StartEvent & ev);
		void	onFixedUpdate(const FixedUpdateEvent & ev);
		void	onUpdate(const UpdateEvent & ev);
		void	onDraw(const DrawEvent & ev);
		void	onGui(const GuiEvent & ev);
		void	onUnload(const UnloadEvent & ev);
		void	onExit(const ExitEvent & ev);

	private:
		bool ML_MainMenuBar_draw();
		bool ML_Dockspace_draw(bool * p_open);
		bool ML_SceneView_draw(bool * p_open);
		bool ML_DemoWindow_draw(bool * p_open);

	private:
		// Batch
		/* * * * * * * * * * * * * * * * * * * * */
		ml::VAO m_batchVAO;
		ml::VBO m_batchVBO;

		ml::HashMap<ml::String, ml::Text> m_text;

		uint32_t m_skybox;

		// Uniform Values
		/* * * * * * * * * * * * * * * * * * * * */
		struct DemoUniforms
		{
			// Time
			float deltaTime;
			float totalTime;

			// Projections
			ml::Transform	persp;
			ml::Transform	ortho;

			// Scene
			ml::vec4f		clearColor = ml::Color::Gray;
			
			// Camera
			ml::Transform	camera;
			bool			camAnimate	= true;
			ml::vec3f		camPos		= { 0, 1, 10 };
			float			camSpd		= 1.f;
			
			// Light
			ml::vec3f		lightPos	= { 0, 1, 30 };
			ml::vec4f		lightCol	= ml::Color::LightYellow;
			float			ambient		= 0.01f;
			float			specular	= 0.5f;
			int32_t			shininess	= 8;
			
			// Framebuffers
			int32_t			effectMode	= 0;
			
			// Geometry
			int32_t			lineMode	= 1;
			ml::vec4f		lineColor	= ml::Color::Red;
			float			lineDelta	= 1.f;
			float			lineSize	= 0.5f;
			int32_t			lineSamples	= 16;
			bool			animate		= true;
		};
		DemoUniforms uni;

		// GUI State
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
			bool show_profiler		= true;
			bool show_shaderTool	= false;
			bool show_network		= false;
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

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_DEMO_HPP_
