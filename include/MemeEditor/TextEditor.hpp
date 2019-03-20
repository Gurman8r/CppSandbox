#ifndef _TEXT_EDITOR_HPP_
#define _TEXT_EDITOR_HPP_

#include <MemeEditor/Document.hpp>

namespace ml
{
	class ML_EDITOR_API TextEditor final
		: public ITrackable
	{
	public:
		TextEditor();
		TextEditor(const String & text);
		TextEditor(CString text, size_t size);
		~TextEditor();

		void draw(CString label, bool * p_open);

	private:
		void draw_menu();
		void draw_tabs();
		bool draw_document(int32_t index);

	private:
		inline Document * get_doc(const size_t index)
		{
			return ((!m_files.empty())
				? ((index < m_files.size())
					? (&m_files.at(index))
					: (NULL))
				: (NULL));
		}

		inline Document * get_selected_doc()
		{
			return get_doc((size_t)m_selected);
		}

	private:
		int32_t m_selected;
		std::vector<Document> m_files;
	};
}

#endif // !_TEXT_EDITOR_HPP_