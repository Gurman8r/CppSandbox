#ifndef _EDITOR_HPP_
#define _EDITOR_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/IEventListener.hpp>

#define ML_Editor ml::Editor::getInstance()

namespace ml
{

	class ML_EDITOR_API Editor final
		: public ITrackable
		, public ISingleton<Editor>
		, public IEventListener
	{
		friend class ISingleton<Editor>;

	private:
		Editor();
		~Editor();

	public:
		void onEvent(const IEvent * value) override;
	};
}

#endif // !_EDITOR_HPP_
