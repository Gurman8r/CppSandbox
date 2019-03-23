#ifndef _ML_MAIN_MENU_BAR_HPP_
#define _ML_MAIN_MENU_BAR_HPP_

#include <MemeEditor/GUI_Tool.hpp>

#define ML_MainMenuBar ml::MainMenuBar::getInstance()

namespace ml
{
	class ML_EDITOR_API MainMenuBar
		: public GUI_Tool
		, public ISingleton<MainMenuBar>
	{
		friend class ISingleton<MainMenuBar>;

	private:
		MainMenuBar();
		~MainMenuBar();

	public:
		bool beginDraw() override;
		bool endDraw() override;
	};
}

#endif // !_ML_MAIN_MENU_BAR_HPP_
