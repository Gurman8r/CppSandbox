#ifndef _ML_PROJECT_VIEW_HPP_
#define _ML_PROJECT_VIEW_HPP_

#include <MemeEditor/GUI_Window.hpp>

#define ML_ResourceHUD ml::ResourceHUD::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ResourceHUD
		: public GUI_Window
		, public ISingleton<ResourceHUD>
	{
		friend class ISingleton<ResourceHUD>;

	private:
		ResourceHUD();
		~ResourceHUD();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	private:
		struct Funcs;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PROJECT_VIEW_HPP_