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

		using TextTable = typename ml::HashMap<ml::String, ml::Text>;

		ml::SStream		m_rdstr;
		ml::StreamBuf * m_rdbuf = NULL;
		ml::Camera		m_camera;
		ml::Canvas		m_canvas;
		TextTable		m_text;

		struct DemoData
		{
			// Scene
			ml::vec4f		clearColor	= { 0.025f, 0.025f, 0.025f, 1.0f };
			
			// Camera
			bool			camAnim		= true;
			float			camSpd		= 1.0f;
			
			// Light
			ml::vec3f		lightPos	= { 0, 1, 30 };
			ml::vec4f		lightCol	= ml::Color::LightYellow;
			float			ambient		= 0.01f;
			float			specular	= 0.5f;
			int32_t			shininess	= 8;
			
			// Effects
			int32_t			effectMode	= 0;
			
			// Geometry
			int32_t			lineMode	= 1;
			ml::vec4f		lineColor	= ml::Color::Red;
			float			lineDelta	= 1.0f;
			float			lineSize	= 0.5f;
			int32_t			lineSamples	= 16;
		};
		DemoData data;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SANDBOX_HPP_