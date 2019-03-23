#ifndef _ML_GUI_BASE_HPP_
#define _ML_GUI_BASE_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_EDITOR_API GUI_Base
		: public ITrackable
	{
	public:
		GUI_Base();
		virtual ~GUI_Base();

	public:
		inline bool	good() const 
		{ 
			return (m_good);
		}

		inline bool init()
		{
			return m_init ? !(m_init = false) : m_init;
		}

	protected:
		inline bool goodCheck(const bool value)
		{ 
			return (m_good = value); 
		}

	private:
		bool m_good;
		bool m_init;
	};
}

#endif // !_ML_GUI_BASE_HPP_