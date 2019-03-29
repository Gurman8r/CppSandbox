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
		void	print(SStream & value);
		void	print(const String & value);
		void    printf(CString value, ...);
		void    execCommand(CString value);
		int32_t textEditCallback(void * value);

	public:
		inline const List<char *> & history() const { return m_history; }

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