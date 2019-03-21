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
		using MyCallback = void(*)();

	public:
		Editor();
		virtual ~Editor();

		bool beginWindow(CString title, bool * p_open, int32_t flags = 0);
		void endWindow();

		virtual bool draw(CString title, bool * p_open);

	private:
		bool m_good = false;
	};
}

#endif // !_EDITOR_HPP_