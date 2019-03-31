#ifndef _DEMO_HPP_
#define _DEMO_HPP_

#include <MemeEditor/EditorApplication.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemePhysics/Particle.hpp>

namespace DEMO
{
	class Demo final
		: public ml::EditorApplication
	{
	public:
		Demo();
		~Demo();

	public:
		void onEvent(const ml::IEvent * value) override;

	private:
		void onEnter		(const ml::EnterEvent		* ev) override;
		void onLoad			(const ml::LoadEvent		* ev) override;
		void onStart		(const ml::StartEvent		* ev) override;
		void onFixedUpdate	(const ml::FixedUpdateEvent * ev) override;
		void onUpdate		(const ml::UpdateEvent		* ev) override;
		void onDraw			(const ml::DrawEvent		* ev) override;
		void onGui			(const ml::GuiEvent			* ev) override;
		void onUnload		(const ml::UnloadEvent		* ev) override;
		void onExit			(const ml::ExitEvent		* ev) override;

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

		// GUI
		/* * * * * * * * * * * * * * * * * * * * */
		struct DemoGUI
		{
			/* * * * * * * * * * * * * * * * * * * * */
			bool show_main_menu_bar = true;
			bool show_dockspace		= true;
			/* * * * * * * * * * * * * * * * * * * * */
			bool show_imgui_demo	= false;
			bool show_imgui_metrics	= false;
			bool show_imgui_style	= false;
			bool show_imgui_about	= false;
			/* * * * * * * * * * * * * * * * * * * * */
			bool show_terminal		= true;
			bool show_browser		= true;
			bool show_builder		= true;
			bool show_inspector		= true;
			bool show_scene			= true;
			bool show_texteditor	= true;
			bool show_hierarchy		= true;
			bool show_resources		= true;
			bool show_profiler		= true;
			bool show_network		= false;
			bool show_demowindow	= true;
			/* * * * * * * * * * * * * * * * * * * * */
		};
		DemoGUI gui;

		// Physics
		/* * * * * * * * * * * * * * * * * * * * */
		struct DemoPhysics
		{
			std::vector<ml::Particle> particles;
		};
		DemoPhysics phys;

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_DEMO_HPP_