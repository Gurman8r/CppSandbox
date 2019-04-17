#ifndef _SANDBOX_HPP_
#define _SANDBOX_HPP_

#include <MemeEditor/EditorApplication.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/Camera.hpp>
#include <MemeGraphics/Canvas.hpp>

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Sandbox final
		: public ml::EditorApplication
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Sandbox();
		~Sandbox();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEvent(const ml::IEvent * value) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEnter	(const ml::EnterEvent	* ev) override;
		void onLoad		(const ml::LoadEvent	* ev) override;
		void onStart	(const ml::StartEvent	* ev) override;
		void onUpdate	(const ml::UpdateEvent	* ev) override;
		void onDraw		(const ml::DrawEvent	* ev) override;
		void onGui		(const ml::GuiEvent		* ev) override;
		void onUnload	(const ml::UnloadEvent	* ev) override;
		void onExit		(const ml::ExitEvent	* ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		bool ML_MainMenuBar_draw();
		bool ML_Dockspace_draw(bool * p_open);
		bool ML_SceneView_draw(bool * p_open);
		bool ML_Inspector_draw(bool * p_open);

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		// Canvas
		ml::Canvas m_canvas;

		// Text
		ml::HashMap<ml::String, ml::Text> m_text;

		// Uniform Values
		struct DemoUniforms
		{
			// Projections
			ml::Transform	persp;
			ml::Transform	ortho;

			// Scene
			ml::vec4f		clearColor = ml::Color::Gray;
			
			// Camera
			ml::Camera		camera;
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

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SANDBOX_HPP_