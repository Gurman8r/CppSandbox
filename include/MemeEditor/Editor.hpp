#ifndef _EDITOR_HPP_
#define _EDITOR_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/Transform.hpp>

#define ML_Editor ml::Editor::getInstance()

namespace ml
{
	class ML_EDITOR_API Editor
		: public ITrackable
		, public ISingleton<Editor>
	{
		friend class ISingleton<Editor>;
		
	private:
		Editor();
		~Editor();

	public:
		void ShowHelpMarker(const char * desc);

		Transform & InputTransform(const char * label, Transform & value);

	private:
	};
}

#endif // !_EDITOR_HPP_
