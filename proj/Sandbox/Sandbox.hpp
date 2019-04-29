#ifndef _SANDBOX_HPP_
#define _SANDBOX_HPP_

#include <MemeEditor/EditorApplication.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/Camera.hpp>
#include <MemeGraphics/Canvas.hpp>
#include <MemeGraphics/Light.hpp>
#include <MemePhysics/PhysicsEvents.hpp>

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
		ml::Canvas		m_canvas;
		TextTable		m_text;

		struct Globals
		{
			// Camera
			bool		camAuto		= true;
			float		camSpd		= 1.0f;
			
			// Effects
			int32_t		effectMode	= 0;
		};
		Globals globals;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SANDBOX_HPP_