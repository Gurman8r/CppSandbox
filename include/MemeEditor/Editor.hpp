#ifndef _EDITOR_HPP_
#define _EDITOR_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	// Base Editor Window Class
	class ML_EDITOR_API Editor
		: public ITrackable
	{
	public:
		Editor();
		virtual ~Editor();

		virtual bool beginWindow(CString title, bool * p_open, int32_t flags = 0);
		virtual bool endWindow();

		virtual bool draw(CString title, bool * p_open) = 0;

	private:
		bool m_good = false;
	};
}

#endif // !_EDITOR_HPP_