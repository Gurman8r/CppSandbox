#include <MemeEditor/MainMenuBar.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MainMenuBar::MainMenuBar()
	{
	}

	MainMenuBar::~MainMenuBar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool MainMenuBar::beginDraw()
	{
		return (m_good = ImGui::BeginMainMenuBar());
	}

	bool MainMenuBar::endDraw()
	{
		ImGui::EndMainMenuBar();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}