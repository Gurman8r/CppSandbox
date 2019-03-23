#ifndef _ML_SCENE_VIEW_HPP_
#define _ML_SCENE_VIEW_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeGraphics/Texture.hpp>

#define ML_SceneView ml::SceneView::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API SceneView
		: public GUI_Window
		, public ISingleton<SceneView>
	{
		friend class ISingleton<SceneView>;

	private:
		SceneView();
		~SceneView();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	protected:
		bool beginDraw(bool * p_open, int32_t flags) override;
		bool endDraw() override;

	public:
		bool updateScene(const Texture * texture);
		bool updateScene(Texture * texture);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCENE_VIEW_HPP_