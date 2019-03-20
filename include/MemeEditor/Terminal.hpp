#ifndef _EXAMPLE_APP_CONSOLE_HPP_
#define _EXAMPLE_APP_CONSOLE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/List.hpp>
#include <imgui/imgui.h>

#define ML_Terminal ml::Terminal::getInstance()

namespace ml
{
	// Command Line Interface
	class ML_EDITOR_API Terminal final
		: public ITrackable
		, public ISingleton<Terminal>
	{
		friend class ISingleton<Terminal>;
	
	public:
		void	setup();
		void    clear();
		void	print(const String & str);
		void    printf(CString fmt, ...) IM_FMTARGS(2);
		void	printHistory();
		void    draw(bool * p_open);
		void    execCommand(CString command_line);
		int32_t textEditCallback(ImGuiInputTextCallbackData* data);

	private:
		char				m_inputBuf[256];
		List<String>		m_lines;
		bool				m_scrollToBottom;
		ImVector<char *>	m_history;
		int32_t				m_historyPos;
		ImVector<CString>	m_auto;

		bool * m_open = NULL;

		Terminal();
		~Terminal();
	};

}

#endif // !_EXAMPLE_APP_CONSOLE_HPP_
