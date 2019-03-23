#ifndef _ML_TERMINAL_HPP_
#define _ML_TERMINAL_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeCore/List.hpp>

#define ML_Terminal ml::Terminal::getInstance()

namespace ml
{
	// Command Line Interface
	class ML_EDITOR_API Terminal final
		: public GUI_Window
		, public ISingleton<Terminal>
	{
		friend class ISingleton<Terminal>;
	
	private:
		Terminal();
		~Terminal();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	public:
		void    clear();
		void	print(const String & str);
		void    printf(CString fmt, ...);
		void	printHistory();
		void    execCommand(CString command_line);
		int32_t textEditCallback(void * value);

	private:
		char			m_inputBuf[256];
		List<String>	m_lines;
		bool			m_scrollToBottom;
		List<char *>	m_history;
		int32_t			m_historyPos;
		List<CString>	m_auto;
	};

}

#endif // !_ML_TERMINAL_HPP_