#ifndef _ML_LOCK_HPP_
#define _ML_LOCK_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Mutex = typename std::mutex;

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class M
	> using UniqueLock = typename std::unique_lock<M>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class M> 
	struct Lock
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using mutex_type = typename M;
		using lock_type = typename UniqueLock<M>;

		/* * * * * * * * * * * * * * * * * * * * */

		mutex_type	mtx;
		lock_type	lck;

		/* * * * * * * * * * * * * * * * * * * * */

		Lock()
			: mtx()
			, lck(mtx)
		{
		}

		virtual ~Lock() {}

		/* * * * * * * * * * * * * * * * * * * * */

		virtual void lock() = 0;
		virtual void unlock() = 0;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct MutexLock final 
		: public Lock<Mutex>
	{
		inline void lock() override
		{
			this->mtx.lock();
		}

		inline void unlock() override
		{
			this->mtx.unlock();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LOCK_HPP_