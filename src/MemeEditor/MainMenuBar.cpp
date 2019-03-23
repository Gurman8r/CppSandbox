#include <MemeEditor/MainMenuBar.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MainMenuBar::MainMenuBar()
		: base_type()
	{
	}

	MainMenuBar::~MainMenuBar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool MainMenuBar::beginDraw()
	{
		return goodCheck(ImGui::BeginMainMenuBar());
	}

	bool MainMenuBar::endDraw()
	{
		ImGui::EndMainMenuBar();
		return good();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}