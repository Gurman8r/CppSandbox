#include <MemeEditor/Terminal.hpp>
#include <MemeScript/Interpreter.hpp>

namespace ml
{
	// Portable helpers
	static int32_t  Stricmp(CString str1, CString str2) { int32_t d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int32_t  Strnicmp(CString str1, CString str2, int32_t n) { int32_t d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char *	Strdup(CString str) { size_t len = strlen(str) + 1; void * buff = malloc(len); return (char *)memcpy(buff, (const void *)str, len); }
	static void		Strtrim(char * str) { char * str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Terminal::Terminal()
	{
	}
	
	Terminal::~Terminal()
	{
		clear();
		
		for (int32_t i = 0; i < m_history.Size; i++)
		{
			free(m_history[i]);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Terminal::setup()
	{
		static bool checked = false;
		if (!checked) checked = true;
		else return;

		clear();
		memset(m_inputBuf, 0, sizeof(m_inputBuf));
		m_historyPos = -1;

		this->printf("# Using this feature may result in crashes or system instability.");
		this->printf("# Type \'help\' for a list of commands.");

		using CommandMap = typename Interpreter::CommandMap;
		using const_iterator = typename CommandMap::const_iterator;

		const CommandMap & commands = ML_Interpreter.commands();

		for (const_iterator it = commands.begin(); it != commands.end(); it++)
		{
			m_auto.push_back(it->first.c_str());
		}
	}

	void Terminal::clear()
	{
		m_lines.clear();
		m_scrollToBottom = true;
	}

	void Terminal::printf(CString fmt, ...)
	{
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		m_lines.push_back(Strdup(buf));
		m_scrollToBottom = true;
	}

	void Terminal::printHistory()
	{
		for (auto h : m_history) { this->printf(h); }
	}

	void Terminal::draw(CString title, bool* p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(550, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close Console"))
			{
				*p_open = false;
			}
			ImGui::EndPopup();
		}


		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		static ImGuiTextFilter filter;
		filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		ImGui::PopStyleVar();
		ImGui::Separator();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear"))
			{
				clear();
			}
			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		
		ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
		for (size_t i = 0; i < m_lines.size(); i++)
		{
			CString item = m_lines[i].c_str();
			if (!filter.PassFilter(item))
				continue;
			ImVec4 col = col_default_text;
			if (strstr(item, "[ LOG ]")) col = ImColor(0.0f, 1.0f, 0.4f, 1.0f);
			else if (strstr(item, "[ WRN ]")) col = ImColor(1.0f, 1.0f, 0.4f, 1.0f);
			else if (strstr(item, "[ ERR ]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
			else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted(item);
			ImGui::PopStyleColor();
		}
		
		if (m_scrollToBottom) 
		{ 
			ImGui::SetScrollHereY(1.0f);
		}

		m_scrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		bool reclaim_focus = false;
		if (ImGui::InputText(
			"Input",
			m_inputBuf,
			IM_ARRAYSIZE(m_inputBuf),
			(
			ImGuiInputTextFlags_EnterReturnsTrue |
			ImGuiInputTextFlags_CallbackCompletion |
			ImGuiInputTextFlags_CallbackHistory
			),
			&textEditCallbackStub,
			(void *)this))
		{
			char * s = m_inputBuf;
			Strtrim(s);
			if (s[0])
			{
				execCommand(s);
			}
			strcpy(s, "");
			reclaim_focus = true;
		}

		// Auto-focus on window apparition
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
		{
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
		}

		ImGui::End();
	}

	void Terminal::execCommand(CString value)
	{
		this->printf("# %s\n", value);

		// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
		m_historyPos = -1;
		for (int32_t i = m_history.Size - 1; i >= 0; i--)
		{
			if (Stricmp(m_history[i], value) == 0)
			{
				free(m_history[i]);
				m_history.erase(m_history.begin() + i);
				break;
			}
		}
		m_history.push_back(Strdup(value));

		SStream	stream;
		std::streambuf * old = ml::cout.rdbuf(stream.rdbuf());

		if (!(ML_Interpreter.execCommand(value)).isErrorType())
		{
			String str;
			while (std::getline(stream, str))
			{
				this->printf(str.c_str());
			}
		}
		else
		{
			this->printf("Unknown command: '%s'\n", value);
		}
		ml::cout.rdbuf(old);
	}

	int32_t Terminal::textEditCallback(ImGuiInputTextCallbackData* data)
	{
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			CString word_end = data->Buf + data->CursorPos;
			CString word_start = word_end;
			while (word_start > data->Buf)
			{
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
				{
					break;
				}
				word_start--;
			}

			// Build a list of candidates
			ImVector<CString> candidates;
			for (int32_t i = 0; i < m_auto.Size; i++)
				if (Strnicmp(m_auto[i], word_start, (int32_t)(word_end - word_start)) == 0)
					candidates.push_back(m_auto[i]);

			if (candidates.Size == 0)
			{
				// No match
				this->printf("No match for \"%.*s\"!\n", (int32_t)(word_end - word_start), word_start);
			}
			else if (candidates.Size == 1)
			{
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
				data->DeleteChars((int32_t)(word_start - data->Buf), (int32_t)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
				int32_t match_len = (int32_t)(word_end - word_start);
				while (true)
				{
					int32_t c = 0;
					bool all_candidates_matches = true;
					for (int32_t i = 0; i < candidates.Size && all_candidates_matches; i++)
					{
						if (i == 0)
						{
							c = toupper(candidates[i][match_len]);
						}
						else if (c == 0 || c != toupper(candidates[i][match_len]))
						{
							all_candidates_matches = false;
						}
					}

					if (!all_candidates_matches)
					{
						break;
					}
					match_len++;
				}

				if (match_len > 0)
				{
					data->DeleteChars((int32_t)(word_start - data->Buf), (int32_t)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}

				// List matches
				this->printf("Possible matches:\n");
				
				for (int32_t i = 0; i < candidates.Size; i++)
				{
					this->printf("- %s\n", candidates[i]);
				}
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int32_t prev_history_pos = m_historyPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (m_historyPos == -1)
				{
					m_historyPos = m_history.Size - 1;
				}
				else if (m_historyPos > 0)
				{
					m_historyPos--;
				}
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (m_historyPos != -1)
				{
					if (++m_historyPos >= m_history.Size)
					{
						m_historyPos = -1;
					}
				}
			}

			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != m_historyPos)
			{
				CString history_str = (m_historyPos >= 0) ? m_history[m_historyPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
		}
		return 0;
	}

	int32_t	Terminal::textEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
	{
		Terminal * console = (Terminal*)data->UserData;

		return console->textEditCallback(data);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}