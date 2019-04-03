#ifndef _ML_LOCK_HPP_
#define _ML_LOCK_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Mutex = typename std::mutex;

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class Mtx
	> using UniqueLock = typename std::unique_lock<Mtx>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class Mtx
	> struct Lock
		: public ITrackable
		, public INonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using mutex_type = typename Mtx;
		using lock_type  = typename UniqueLock<Mtx>;

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