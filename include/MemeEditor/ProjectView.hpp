#ifndef _ML_PROJECT_VIEW_HPP_
#define _ML_PROJECT_VIEW_HPP_

#include <MemeEditor/GUI_Window.hpp>

#define ML_ProjectView ml::ProjectView::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ProjectView
		: public GUI_Window
		, public ISingleton<ProjectView>
	{
		friend class ISingleton<ProjectView>;

	private:
		ProjectView();
		~ProjectView();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	private:
		struct Funcs;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PROJECT_VIEW_HPP_