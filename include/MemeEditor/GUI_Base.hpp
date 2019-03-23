#ifndef _ML_GUI_BASE_HPP_
#define _ML_GUI_BASE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	// Base class for GUI
	class ML_EDITOR_API GUI_Base
		: public ITrackable
	{
	public:
		GUI_Base();
		virtual ~GUI_Base();

	public:
		virtual bool beginDraw(CString title, bool * p_open, int32_t flags = 0);
		virtual bool endDraw();

	public:
		uint32_t getID() const;

	public:
		inline bool		good()	const { return m_good; }
		inline CString	title() const { return m_title; }
		inline bool *	open()	const { return m_open; }
		inline int32_t	flags() const { return m_flags; }

	protected:
		bool	m_good;
		CString	m_title;
		bool *	m_open;
		int32_t m_flags;
	};
}

#endif // !_ML_GUI_BASE_HPP_