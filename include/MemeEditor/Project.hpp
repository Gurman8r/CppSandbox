#ifndef _ML_PROJECT_VIEW_HPP_
#define _ML_PROJECT_VIEW_HPP_

#include <MemeEditor/GUI_Window.hpp>

#define ML_Project ml::Project::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Project final
		: public GUI_Window
		, public ISingleton<Project>
	{
		friend class ISingleton<Project>;

	private:
		Project();
		~Project();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	private:
		struct Funcs;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PROJECT_VIEW_HPP_