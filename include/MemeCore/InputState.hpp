#ifndef _INPUT_STATE_H_
#define _INPUT_STATE_H_

#include <MemeCore/IntTypes.hpp>

namespace ml
{
	template <size_t N>
	class InputState
	{
	public:
		using size_type = size_t;

		enum : size_t { Size = N };
		
		using self_type = InputState<Size>;

	public:
		virtual ~InputState() { }

	public:
		virtual bool checkKey(int32_t value) const = 0;

	public:
		self_type & beginStep()
		{
			for (size_t i = 0; i < Size; i++)
			{
				m_new[i] = checkKey((int32_t)i);
			}
			return (*this);
		}

		self_type & endStep()
		{
			for (size_t i = 0; i < Size; i++)
			{
				m_old[i] = m_new[i];
			}
			return (*this);
		}

	public:
		inline bool getKey(int32_t value) const
		{
			return (m_new[value]);
		}

		inline bool getKeyDown(int32_t value) const
		{
			return (m_new[value]) && (!m_old[value]);
		}

		inline bool getKeyUp(int32_t value) const
		{
			return (!m_new[value]) && (m_old[value]);
		}

	protected:
		bool m_new[N];
		bool m_old[N];
	};
}

#endif // !_INPUT_STATE_H_
