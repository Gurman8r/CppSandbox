#ifndef _DOCKSPACE_HPP_
#define _DOCKSPACE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/Vector2.hpp>

namespace ml
{
	// Docking/snapping area for windows
	class ML_EDITOR_API Dockspace final 
		: public ITrackable
	{
	public:
		using void_fun = void(*)();

	public:
		Dockspace();
		Dockspace(const String & title);
		Dockspace(const String & title, int32_t dock_flags);
		Dockspace(const Dockspace & copy);
		~Dockspace();

	public:
		bool begin_dock(bool * p_open);
		void end_dock();

		uint32_t getID() const;
		CString	 getTitle() const;

	public:
		int32_t dock_flags;
		bool	fullscreen;
		float	border;
		vec2f	padding;
		float	rounding;
		vec2f	size;
		String	title;
		int32_t win_flags;
		float	bgAlpha;

	private:
		bool m_good = false;
	};
}

#endif // !_DOCKSPACE_HPP_
