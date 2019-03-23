#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IEventListener.hpp>

namespace ml
{
	// Base Editor Class
	class ML_EDITOR_API Editor
		: public ITrackable
		, public IEventListener
	{
	public:
		Editor();
		virtual ~Editor();

		virtual void onEvent(const IEvent * value) override;

		virtual bool beginWindow(CString title, bool * p_open, int32_t flags = 0);

		virtual bool endWindow();

		virtual bool draw(CString title, bool * p_open) = 0;

	private:
		bool m_good = false;
	};
}

#endif // !_ML_EDITOR_HPP_