#ifndef _ML_MAIN_MENU_BAR_HPP_
#define _ML_MAIN_MENU_BAR_HPP_

#include <MemeEditor/GUI_Base.hpp>

#define ML_MainMenuBar ml::MainMenuBar::getInstance()

namespace ml
{
	class ML_EDITOR_API MainMenuBar
		: public ITrackable
		, public ISingleton<MainMenuBar>
	{
		friend class ISingleton<MainMenuBar>;

	private:
		MainMenuBar();
		~MainMenuBar();

	public:
		bool beginDraw();
		bool endDraw();

	private:
		bool m_good;
	};
}

#endif // !_ML_MAIN_MENU_BAR_HPP_
