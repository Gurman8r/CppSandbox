#ifndef _ML_DOCKSPACE_HPP_
#define _ML_DOCKSPACE_HPP_

#include <MemeEditor/EditorWindow.hpp>
#include <MemeCore/Vector2.hpp>

#define ML_Dock ml::Dockspace::getInstance()

namespace ml
{
	// Docking/snapping area for windows
	class ML_EDITOR_API Dockspace final 
		: public EditorBase
		, public ISingleton<Dockspace>
	{
		friend class ISingleton<Dockspace>;

	private:
		Dockspace();
		~Dockspace();

	public:
		bool beginDraw(CString title, bool * p_open, int32_t flags = 0) override;
		bool endDraw() override;

	public:
		uint32_t split(uint32_t dir, float ratio, uint32_t * out, uint32_t * other);
		uint32_t split(uint32_t id, int32_t dir, float ratio, uint32_t * other);
		uint32_t split(uint32_t id, int32_t dir, float ratio, uint32_t * out, uint32_t * other);

		void dock_window(CString name, uint32_t id);

	public:
		int32_t dock_flags;
		bool	fullscreen;
		float	border;
		vec2f	padding;
		float	rounding;
		vec2f	size;
		float	bgAlpha;
	};
}

#endif // !_ML_DOCKSPACE_HPP_