#ifndef _ML_BROWSER_HPP_
#define _ML_BROWSER_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Bytes.hpp>

#define ML_Browser ml::Browser::getInstance()

namespace ml
{
	// File Browser
	class ML_EDITOR_API Browser final
		: public GUI_Window
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

	public:
		using Directory = typename FileSystem::Directory;

	public:
		static const Bytes MaxPreviewSize;

	private:
		Browser();
		~Browser();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

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
		String	get_selected_type() const;
		size_t	get_selected_size() const;

		/* * * * * * * * * * * * * * * * * * * * */

		inline String pathTo(const String & value) const 
		{ 
			return (m_path + "\\" + value);
		}

		inline const List<String> * get_list() const
		{
			Directory::const_iterator it;
			return (((it = m_dir.find(m_type)) != m_dir.end())
				? (&it->second)
				: (NULL));
		}

		inline const String * get_selected() const
		{
			const List<String> * list;
			return ((list = get_list())
				? ((m_index < list->size())
					? &(*list).at(m_index)
					: NULL)
				: NULL);
		}

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		String		m_path;		// Working Directory
		Directory	m_dir;		// Directory Contents
		char		m_type;		// Selected List
		size_t		m_index;	// Selected File
		String		m_preview;	// File Contents
		bool		m_isDouble;	// Has Double Click
	};
}
#endif // !_ML_BROWSER_HPP_