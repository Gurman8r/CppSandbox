#ifndef _BROWSER_HPP_
#define _BROWSER_HPP_

#include <MemeEditor/Editor.hpp>
#include <MemeCore/IEventListener.hpp>

namespace ml
{
	class ML_EDITOR_API Browser
		: public ITrackable
		, public IEventListener
	{
	public:
		using Directory	= HashMap<char, List<String>>;

	public:
		Browser();
		~Browser();

		void onEvent(const IEvent * value);

		void update(const String & path);

		void draw(bool * p_open);

	private:
		void draw_file_list();
		void draw_file_data();

		void	setSelected(char type, size_t index, bool isDouble);
		String	selectedFile() const;
		String	selectedType() const;

		inline bool isSelected(char type, size_t index)
		{
			return (m_type == type) && (m_index == index);
		}

	private:
		String		m_path;		// Working Directory
		Directory	m_dir;		// Directory Contents
		char		m_type;		// Selected List
		size_t		m_index;	// Selected File
		File		m_preview;	// File Contents
		bool		m_isDouble;	// Has Double Click
	};
}
#endif // !_BROWSER_HPP_
