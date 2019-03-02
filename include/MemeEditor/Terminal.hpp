#ifndef _EXAMPLE_APP_CONSOLE_HPP_
#define _EXAMPLE_APP_CONSOLE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/List.hpp>
#include <imgui/imgui.h>

#define ML_EditorConsole ml::Terminal::getInstance()

namespace ml
{
	class ML_EDITOR_API Terminal
		: public ITrackable
		, public ISingleton<Terminal>
	{
		friend class ISingleton<Terminal>;

	private:
		char				m_inputBuf[256];
		List<String>		m_lines;
		bool				m_scrollToBottom;
		ImVector<char *>	m_history;
		int32_t				m_historyPos;    // -1: new line, 0..History.Size-1 browsing history.
		ImVector<CString>	m_auto;

		bool m_visible = false;

		Terminal();
		~Terminal();

	public:
		void	setup();
		void    clear();
		void    printf(CString fmt, ...) IM_FMTARGS(2);
		void	printHistory();
		void    draw(CString title, bool* p_open);
		void    execCommand(CString command_line);
		int32_t textEditCallback(ImGuiInputTextCallbackData* data);

		static int32_t textEditCallbackStub(ImGuiInputTextCallbackData* data);

		inline bool &	visible()	{ return m_visible; }
		inline void		draw()		{ return draw("Console", &visible()); }
	};

}

#endif // !_EXAMPLE_APP_CONSOLE_HPP_
