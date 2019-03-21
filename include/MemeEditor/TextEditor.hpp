#ifndef _TEXT_EDITOR_HPP_
#define _TEXT_EDITOR_HPP_

#include <MemeEditor/Editor.hpp>
#include <MemeEditor/Document.hpp>

namespace ml
{
	class ML_EDITOR_API TextEditor final
		: public Editor
	{
	public:
		TextEditor();
		TextEditor(const String & text);
		TextEditor(CString text, size_t size);
		~TextEditor();

		bool draw(CString title, bool * p_open) override;

	private:
		void draw_menu();
		void draw_tabs();
		bool edit_document(int32_t index);
		bool edit_document_name(char * buf, Document * doc);
		bool edit_document_data(Document * doc);

	private:
		inline Document * get_document(const size_t index)
		{
			return ((!m_files.empty())
				? ((index < m_files.size())
					? (&m_files.at(index))
					: (NULL))
				: (NULL));
		}

		inline Document * get_selected_document()
		{
			return get_document((size_t)m_selected);
		}

	private:
		int32_t m_selected;
		std::vector<Document> m_files;
	};
}

#endif // !_TEXT_EDITOR_HPP_