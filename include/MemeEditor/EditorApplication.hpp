#ifndef _ML_EDITOR_APPLICATION_HPP_
#define _ML_EDITOR_APPLICATION_HPP_

#include <MemeEditor/EditorEvents.hpp>
#include <MemeEngine/Application.hpp>

namespace ml
{
	class ML_EDITOR_API EditorApplication
		: public Application
	{
	public:
		EditorApplication();
		virtual ~EditorApplication();

	public:
		virtual void onEvent(const IEvent * value) override;

	public:
		virtual void onEnter	(const EnterEvent	* ev) = 0;
		virtual void onLoad		(const LoadEvent	* ev) = 0;
		virtual void onStart	(const StartEvent	* ev) = 0;
		virtual void onUpdate	(const UpdateEvent	* ev) = 0;
		virtual void onDraw		(const DrawEvent	* ev) = 0;
		virtual void onGui		(const GuiEvent		* ev) = 0;
		virtual void onUnload	(const UnloadEvent	* ev) = 0;
		virtual void onExit		(const ExitEvent	* ev) = 0;
	};
}

#endif // !_ML_EDITOR_APPLICATION_HPP_