#include <MemeEditor/Terminal.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeScript/ScriptEvents.hpp>

# ifndef strdup
# define strdup _strdup
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Terminal::Terminal()
		: GUI_Window("Terminal")
		, m_inputBuf	()
		, m_lines		()
		, m_scrollBottom()
		, m_history		()
		, m_historyPos	(-1)
		, m_autoFill	()
	{
		clear();

		std::memset(m_inputBuf, 0, sizeof(m_inputBuf));

		for (auto & pair : ML_Interpreter.commands())
		{
			m_autoFill.push_back(pair.first.c_str());
		}

		this->printf("# Using this feature may result in crashes or system instability.");
		this->printf("# Type \'help\' for a list of commands.");
	}
	
	Terminal::~Terminal()
	{
		clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Terminal::onEvent(const IEvent * value)
	{
		switch (*value)
		{
		case CoreEvent::EV_FS_ChangeDir:
			if (const auto * ev = value->as<FS_ChangDirEvent>())
			{
			}
			break;
		}
	}

	bool Terminal::drawGui(bool * p_open)
	{
		if (beginDraw(p_open))
		{
			// Filter
			static ImGuiTextFilter filter;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			filter.Draw("Filter (\"incl,-excl\")", 180);
			ImGui::PopStyleVar();
			ImGui::Separator();

			// Text
			const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

			ImGui::BeginChild(
				"ScrollingRegion",
				{ 0, -footer_height },
				false,
				ImGuiWindowFlags_HorizontalScrollbar);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
			ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			for (size_t i = 0; i < m_lines.size(); i++)
			{
				CString item = m_lines[i].c_str();
				if (filter.PassFilter(item))
				{
					ImVec4 col = col_default_text;
					if (std::strstr(item, "[ LOG ]")) col = ImColor(0.0f, 1.0f, 0.4f, 1.0f);
					else if (std::strstr(item, "[ WRN ]")) col = ImColor(1.0f, 1.0f, 0.4f, 1.0f);
					else if (std::strstr(item, "[ ERR ]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
					else if (std::strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
					ImGui::PushStyleColor(ImGuiCol_Text, col);
					ImGui::TextUnformatted(item);
					ImGui::PopStyleColor();
				}
			}

			if (m_scrollBottom)
			{
				ImGui::SetScrollHereY(1.0f);
			}

			m_scrollBottom = false;
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();
			
			// Input
			bool reclaim_focus;
			if (reclaim_focus = ImGui::InputText(
				"Input", 
				m_inputBuf,
				IM_ARRAYSIZE(m_inputBuf),
				(
					ImGuiInputTextFlags_EnterReturnsTrue |
					ImGuiInputTextFlags_CallbackCompletion |
					ImGuiInputTextFlags_CallbackHistory
				),
				[](auto data) { return (static_cast<Terminal *>(data->UserData))->inputCallback(data); },
				static_cast<void *>(this))
			)
			{
				auto strtrim = [](char * str)
				{
					char * str_end = str + std::strlen(str);
					while (str_end > str && str_end[-1] == ' ')
						str_end--;
					*str_end = 0;
					return str;
				};

				char * s = strtrim(m_inputBuf);
				if (s[0])
				{
					execute(s);
				}
				strcpy(s, "");
			}
			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
			{
				// Auto focus previous widget
				ImGui::SetKeyboardFocusHere(-1);
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Terminal::clear()
	{
		m_lines.clear();
		m_scrollBottom = true;
	}

	void Terminal::execute(CString value)
	{
		this->printf("# %s\n", value);

		// Insert into history.
		m_historyPos = -1;
		for (int32_t i = (int32_t)m_history.size() - 1; i >= 0; i--)
		{
			if (std::strcmp(m_history[i], value) == 0)
			{
				free(m_history[i]);
				m_history.erase(m_history.begin() + i);
				break;
			}
		}
		m_history.push_back(strdup(value));

		ML_EventSystem.fireEvent(CommandEvent(value));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Terminal::printf(CString value, ...)
	{
		char buf[1024];
		va_list args;
		va_start(args, value);
		vsnprintf(buf, IM_ARRAYSIZE(buf), value, args);
		buf[IM_ARRAYSIZE(buf) - 1] = '\0';
		va_end(args);
		this->printl(buf);
	}

	void Terminal::printl(const String & value)
	{
		m_lines.push_back(value);
		m_scrollBottom = true;
	}

	void Terminal::printss(SStream & value)
	{
		if (const String & text = value.str())
		{
			SStream sink(text);
			String	line;
			while (std::getline(sink, line))
			{
				this->printl(line);
			}
			value.str(String());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	int32_t Terminal::inputCallback(void * value)
	{
		ImGuiInputTextCallbackData * data;
		if (!(data = (ImGuiInputTextCallbackData *)(value))) 
		{ 
			return 0; 
		}

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
			for (int32_t i = 0; i < (int32_t)m_autoFill.size(); i++)
			{
				if (std::strncmp(m_autoFill[i], word_start, (int32_t)(word_end - word_start)) == 0)
				{
					candidates.push_back(m_autoFill[i]);
				}
			}

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
					m_historyPos = (int32_t)m_history.size() - 1;
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
					if (++m_historyPos >= (int32_t)m_history.size())
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

	/* * * * * * * * * * * * * * * * * * * * */
}