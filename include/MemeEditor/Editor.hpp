#ifndef _EDITOR_HPP_
#define _EDITOR_HPP_

#include <MemeEditor/Export.hpp>

namespace ml
{
	class ML_EDITOR_API Editor
	{
	public:
		Editor();
		~Editor();

		static void ShowHelpMarker(const char * desc);

	private:

	};
}

#endif // !_EDITOR_HPP_
