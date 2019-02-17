#ifndef _EXAMPLE_APP_CONSOLE_HPP_
#define _EXAMPLE_APP_CONSOLE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <imgui/imgui.h>

namespace ml
{
	struct ML_EDITOR_API InterpreterConsole
		: public ITrackable
	{
		char					InputBuf[256];
		ImVector<char *>		Items;
		bool					ScrollToBottom;
		ImVector<char *>		History;
		int						HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
		ImVector<ml::CString>	Commands;

		InterpreterConsole();
		~InterpreterConsole();

		void    ClearLog();
		void    AddLog(ml::CString fmt, ...) IM_FMTARGS(2);
		void    Draw(ml::CString title, bool* p_open);
		void    ExecCommand(ml::CString command_line);
		int     TextEditCallback(ImGuiInputTextCallbackData* data);

		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
	};

	static void ShowInterpreterConsole(bool* p_open)
	{
		static InterpreterConsole console;
		console.Draw("MemeScript Console", p_open);
	}

}

#endif // !_EXAMPLE_APP_CONSOLE_HPP_
