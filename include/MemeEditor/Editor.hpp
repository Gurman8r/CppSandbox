#ifndef _EDITOR_HPP_
#define _EDITOR_HPP_

#include <MemeEditor/Dockspace.hpp>
#include <MemeCore/IEventListener.hpp>

#define ML_Editor ml::Editor::getInstance()

namespace ml
{
	class ML_EDITOR_API Editor
		: public ITrackable
		, public ISingleton<Editor>
		, public IEventListener
	{
		friend class ISingleton<Editor>;

	private:
		Editor();
		~Editor();

	public:
		void onEvent(const IEvent * value);
		void draw(bool * p_open);

	private:
		Dockspace m_dock;
	};
}

#endif // !_EDITOR_HPP_
