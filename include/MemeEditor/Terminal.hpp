#ifndef _EXAMPLE_APP_CONSOLE_HPP_
#define _EXAMPLE_APP_CONSOLE_HPP_

#include <MemeEditor/Editor.hpp>
#include <MemeCore/List.hpp>

#define ML_Terminal ml::Terminal::getInstance()

namespace ml
{
	// Command Line Interface
	class ML_EDITOR_API Terminal final
		: public Editor
		, public ISingleton<Terminal>
	{
		friend class ISingleton<Terminal>;
	
	public:
		void    clear();
		void	print(const String & str);
		void    printf(CString fmt, ...);
		void	printHistory();
		bool	draw(CString title, bool * p_open) override;
		void    execCommand(CString command_line);
		int32_t textEditCallback(void * value);

	private:
		char				m_inputBuf[256];
		List<String>		m_lines;
		bool				m_scrollToBottom;
		List<char *>		m_history;
		int32_t				m_historyPos;
		List<CString>		m_auto;

		bool * m_open = NULL;

		Terminal();
		~Terminal();
	};

}

#endif // !_EXAMPLE_APP_CONSOLE_HPP_
