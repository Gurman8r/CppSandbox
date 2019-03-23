#ifndef _ML_DOCKSPACE_HPP_
#define _ML_DOCKSPACE_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeCore/Vector2.hpp>

#define ML_Dockspace ml::Dockspace::getInstance()

namespace ml
{
	// Docking/snapping area for windows
	class ML_EDITOR_API Dockspace final 
		: public GUI_Window
		, public ISingleton<Dockspace>
	{
		friend class ISingleton<Dockspace>;

	private:
		Dockspace();
		~Dockspace();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	protected:
		bool beginDraw(bool * p_open, int32_t flags = 0) override;
		bool endDraw() override;

	public:
		uint32_t beginBuilder(int32_t flags);
		uint32_t endBuilder(uint32_t root);
		uint32_t dockWindow(CString name, uint32_t id);
		uint32_t splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * other);
		uint32_t splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * out, uint32_t * other);

	public:
		float	border;
		vec2f	padding;
		float	rounding;
		vec2f	size;
		float	bgAlpha;
	};
}

#endif // !_ML_DOCKSPACE_HPP_