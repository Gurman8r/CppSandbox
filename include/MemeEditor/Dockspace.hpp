#ifndef _DOCKSPACE_HPP_
#define _DOCKSPACE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/Vector2.hpp>

namespace ml
{
	// Used for docking windows
	class ML_EDITOR_API Dockspace final : public ITrackable
	{
	public:
		Dockspace();
		Dockspace(const String & title);
		Dockspace(const String & title, int32_t dock_flags);
		Dockspace(const Dockspace & copy);
		~Dockspace();

	public:
		void update();
		
		bool draw(const String & title, bool * p_open);
		
		uint32_t getID() const;

	public:
		int32_t dock_flags;
		bool	fullscreen;
		float	border;
		vec2f	padding;
		float	rounding;
		vec2f	size;
		String	title;
		int32_t win_flags;
	};
}

#endif // !_DOCKSPACE_HPP_
