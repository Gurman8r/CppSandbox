#ifndef _BROWSER_HPP_
#define _BROWSER_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/File.hpp>
#include <MemeCore/IEventListener.hpp>
#include <MemeCore/Bytes.hpp>

#define ML_Browser ml::Browser::getInstance()

namespace ml
{
	class ML_EDITOR_API Browser
		: public ITrackable
		, public IEventListener
		, public ISingleton<Browser>
	{
		friend class ISingleton<Browser>;

	public:
		enum : char
		{
			T_Reg = ' ',
			T_Dir = '/',
			T_Lnk = '@',
			T_Unk = '*',
		};

		using Directory = HashMap<char, List<String>>;

		static const Bytes MaxPreviewSize;

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		Browser();
		~Browser();

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		void onEvent(const IEvent * value) override;

		void draw(bool * p_open);

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		void draw_menu();
		void draw_directory();
		void draw_file();
		void draw_file_preview();
		void draw_file_details();

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		void	set_selected(char type, size_t index);
		String	get_selected_name() const;
		String	get_selected_path() const;
		String	get_selected_ext() const;
		size_t	get_selected_size() const;

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		inline String pathTo(const String & value) const
		{
			return (m_path + "\\" + value);
		}

		inline const List<String> * getList() const
		{
			Directory::const_iterator it;
			return (((it = m_dir.find(m_type)) != m_dir.end())
				? (&it->second)
				: (NULL));
		}

		inline const String * getFile() const
		{
			const List<String> * list;
			return ((list = getList())
				? ((m_index < list->size())
					? &(*list).at(m_index)
					: NULL)
				: NULL);
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		bool *		m_open;		// Is Open?
		String		m_path;		// Working Directory
		Directory	m_dir;		// Directory Contents
		char		m_type;		// Selected List
		size_t		m_index;	// Selected File
		String		m_preview;	// File Contents
		bool		m_isDouble;	// Has Double Click

		/* * * * * * * * * * * * * * * * * * * * */
	};
}
#endif // !_BROWSER_HPP_
