#ifndef _EXAMPLE_APP_CONSOLE_HPP_
#define _EXAMPLE_APP_CONSOLE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <imgui/imgui.h>

#define ML_EditorConsole ml::EditorConsole::getInstance()

namespace ml
{
	class ML_EDITOR_API EditorConsole
		: public ITrackable
		, public ISingleton<EditorConsole>
	{
		friend class ISingleton<EditorConsole>;

	private:
		char					m_inputBuf[256];
		ImVector<char *>		m_lines;
		bool					m_scrollToBottom;
		ImVector<char *>		m_history;
		int32_t					m_historyPos;    // -1: new line, 0..History.Size-1 browsing history.
		ImVector<CString>		m_auto;

		EditorConsole();
		~EditorConsole();

	public:
		void	setup();
		void    clear();
		void    printf(CString fmt, ...) IM_FMTARGS(2);
		void    draw(CString title, bool* p_open);
		void    execCommand(CString command_line);
		int32_t textEditCallback(ImGuiInputTextCallbackData* data);

		static int32_t textEditCallbackStub(ImGuiInputTextCallbackData* data);
	};

}

#endif // !_EXAMPLE_APP_CONSOLE_HPP_
