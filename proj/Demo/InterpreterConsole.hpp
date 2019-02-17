#ifndef _EXAMPLE_APP_CONSOLE_HPP_
#define _EXAMPLE_APP_CONSOLE_HPP_

#include <imgui/imgui.h>
#include <MemeScript/Interpreter.hpp>
#include "Settings.hpp"

namespace ml
{
	struct InterpreterConsole
	{
		char                  InputBuf[256];
		ImVector<char *>       Items;
		bool                  ScrollToBottom;
		ImVector<char *>       History;
		int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
		ImVector<ml::CString> Commands;

		InterpreterConsole()
		{
			ClearLog();
			memset(InputBuf, 0, sizeof(InputBuf));
			HistoryPos = -1;
			AddLog("# Using this feature may result in crashes or system instability.");
			AddLog("# Type \'help\' for a list of commands.");
			ExecCommand(ml::String("exec " + SETTINGS.pathTo(SETTINGS.scrPath + SETTINGS.scrFile)).c_str());

			Commands.push_back("clearChildren");
			Commands.push_back("command");
			Commands.push_back("if");
			Commands.push_back("elif");
			Commands.push_back("else");
			Commands.push_back("for");
			Commands.push_back("while");
			Commands.push_back("print");
			Commands.push_back("delete");
			Commands.push_back("include");
			Commands.push_back("return");
			for (const std::string & e : ML_Interpreter.cmdNames())
			{
				Commands.push_back(e.c_str());
			}

		}
		~InterpreterConsole()
		{
			ClearLog();
			for (int i = 0; i < History.Size; i++)
				free(History[i]);
		}

		// Portable helpers
		static int   Stricmp(ml::CString str1, ml::CString str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
		static int   Strnicmp(ml::CString str1, ml::CString str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
		static char *Strdup(ml::CString str) { size_t len = strlen(str) + 1; void * buff = malloc(len); return (char *)memcpy(buff, (const void *)str, len); }
		static void  Strtrim(char * str) { char * str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

		void    ClearLog()
		{
			for (int i = 0; i < Items.Size; i++)
				free(Items[i]);
			Items.clear();
			ScrollToBottom = true;
		}

		void    AddLog(ml::CString fmt, ...) IM_FMTARGS(2)
		{
			// FIXME-OPT
			char buf[1024];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
			buf[IM_ARRAYSIZE(buf) - 1] = 0;
			va_end(args);
			Items.push_back(Strdup(buf));
			ScrollToBottom = true;
		}

		void    Draw(ml::CString title, bool* p_open)
		{
			ImGui::SetNextWindowSize(ImVec2(550, 600), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin(title, p_open))
			{
				ImGui::End();
				return;
			}

			// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
			// Here we create a context menu only available from the title bar.
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Close Console"))
					*p_open = false;
				ImGui::EndPopup();
			}

			//ImGui::TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
			if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
			bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
			if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
			//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

			ImGui::Separator();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			static ImGuiTextFilter filter;
			filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
			ImGui::PopStyleVar();
			ImGui::Separator();

			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Clear")) ClearLog();
				ImGui::EndPopup();
			}

			// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
			// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
			// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
			// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
			//     ImGuiListClipper clipper(Items.Size);
			//     while (clipper.Step())
			//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			// However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
			// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
			// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
			// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			if (copy_to_clipboard)
				ImGui::LogToClipboard();
			ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			for (int i = 0; i < Items.Size; i++)
			{
				ml::CString item = Items[i];
				if (!filter.PassFilter(item))
					continue;
				ImVec4 col = col_default_text;
				if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
				else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
				ImGui::PushStyleColor(ImGuiCol_Text, col);
				ImGui::TextUnformatted(item);
				ImGui::PopStyleColor();
			}
			if (copy_to_clipboard)
				ImGui::LogFinish();
			if (ScrollToBottom)
				ImGui::SetScrollHereY(1.0f);
			ScrollToBottom = false;
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			// Command-line
			bool reclaim_focus = false;
			if (ImGui::InputText(
				"Input", 
				InputBuf, 
				IM_ARRAYSIZE(InputBuf), 
				(
					ImGuiInputTextFlags_EnterReturnsTrue |
					ImGuiInputTextFlags_CallbackCompletion |
					ImGuiInputTextFlags_CallbackHistory
				),
				&TextEditCallbackStub, 
				(void *)this))
			{
				char * s = InputBuf;
				Strtrim(s);
				if (s[0])
					ExecCommand(s);
				strcpy(s, "");
				reclaim_focus = true;
			}

			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			ImGui::End();
		}

		void    ExecCommand(ml::CString command_line)
		{
			AddLog("# %s\n", command_line);

			// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
			HistoryPos = -1;
			for (int i = History.Size - 1; i >= 0; i--)
				if (Stricmp(History[i], command_line) == 0)
				{
					free(History[i]);
					History.erase(History.begin() + i);
					break;
				}
			History.push_back(Strdup(command_line));

			if (String(command_line) == "clearChildren")
			{
				ClearLog();
			}
			else
			{
				std::stringstream   redirectStream;
				std::streambuf*     oldbuf = std::cout.rdbuf(redirectStream.rdbuf());
				ml::Var var;
				if (!(ML_Interpreter.execCommand(command_line)).isErrorType())
				{
					std::string str;
					while (std::getline(redirectStream, str))
					{
						AddLog(str.c_str());
					}
				}
				else
				{
					AddLog("Unknown command: '%s'\n", command_line);
				}
				std::cout.rdbuf(oldbuf);
			}
		}

		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
		{
			InterpreterConsole* console = (InterpreterConsole*)data->UserData;
			return console->TextEditCallback(data);
		}

		int     TextEditCallback(ImGuiInputTextCallbackData* data)
		{
			//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				// Example of TEXT COMPLETION

				// Locate beginning of current word
				ml::CString word_end = data->Buf + data->CursorPos;
				ml::CString word_start = word_end;
				while (word_start > data->Buf)
				{
					const char c = word_start[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;
					word_start--;
				}

				// Build a list of candidates
				ImVector<ml::CString> candidates;
				for (int i = 0; i < Commands.Size; i++)
					if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
						candidates.push_back(Commands[i]);

				if (candidates.Size == 0)
				{
					// No match
					AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
				}
				else if (candidates.Size == 1)
				{
					// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				}
				else
				{
					// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
					int match_len = (int)(word_end - word_start);
					for (;;)
					{
						int c = 0;
						bool all_candidates_matches = true;
						for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c == 0 || c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						if (!all_candidates_matches)
							break;
						match_len++;
					}

					if (match_len > 0)
					{
						data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}

					// List matches
					AddLog("Possible matches:\n");
					for (int i = 0; i < candidates.Size; i++)
						AddLog("- %s\n", candidates[i]);
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				// Example of HISTORY
				const int prev_history_pos = HistoryPos;
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (HistoryPos == -1)
						HistoryPos = History.Size - 1;
					else if (HistoryPos > 0)
						HistoryPos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (HistoryPos != -1)
						if (++HistoryPos >= History.Size)
							HistoryPos = -1;
				}

				// A better implementation would preserve the data on the current input line along with cursor position.
				if (prev_history_pos != HistoryPos)
				{
					ml::CString history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, history_str);
				}
			}
			}
			return 0;
		}
	};

	static void ShowInterpreterConsole(bool* p_open)
	{
		static InterpreterConsole console;
		console.Draw("MemeScript Console", p_open);
	}

}

#endif // !_EXAMPLE_APP_CONSOLE_HPP_