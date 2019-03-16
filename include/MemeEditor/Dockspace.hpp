#ifndef _DOCKSPACE_HPP_
#define _DOCKSPACE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_EDITOR_API Dockspace
	{
	public:
		Dockspace();
		~Dockspace();

	public:
		void draw(bool * p_open);

	public:
		int32_t opt_flags;
		bool	opt_fullscreen;
		bool	opt_fullscreen_persistant;
		int32_t window_flags;
	};
}

#endif // !_DOCKSPACE_HPP_
