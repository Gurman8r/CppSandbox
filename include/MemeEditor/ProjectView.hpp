#ifndef _ML_PROJECT_VIEW_HPP_
#define _ML_PROJECT_VIEW_HPP_

#include <MemeEditor/GUI_Window.hpp>

#define ML_ProjectView ml::ProjectView::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ProjectView final
		: public GUI_Window
		, public ISingleton<ProjectView>
	{
		friend class ISingleton<ProjectView>;

	private:
		ProjectView();
		~ProjectView();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	private:
		struct Funcs;

		void draw_effects	();
		void draw_entities	();
		void draw_fonts		();
		void draw_images	();
		void draw_lua		();
		void draw_mats		();
		void draw_meshes	();
		void draw_models	();
		void draw_plugins	();
		void draw_scripts	();
		void draw_shaders	();
		void draw_skyboxes	();
		void draw_sounds	();
		void draw_sprites	();
		void draw_textures	();
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PROJECT_VIEW_HPP_