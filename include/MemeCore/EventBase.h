#ifndef _EVENT_BASE_H_
#define _EVENT_BASE_H_

#include <MemeCore/ITrackable.h>

namespace ml
{
	class ML_CORE_API EventBase
		: public ITrackable
	{
	public:
		enum EventID : int
		{
			EV_Invalid = -1,
			MAX_EVENT_ID
		};

	public:
		EventBase(int id)
			: m_id(id)
		{
		}
		virtual ~EventBase() {}

		inline const int & id() const { return m_id; }

	private:
		int m_id;

	};
}

#endif // !_EVENT_BASE_H_
