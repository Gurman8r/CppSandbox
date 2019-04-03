#ifndef _ML_PHYSICS_THREAD_HPP_
#define _ML_PHYSICS_THREAD_HPP_

#include <MemeCore/Lock.hpp>
#include <MemePhysics/Export.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeCore/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API PhysicsThread final
		: public ITrackable
		, public IDisposable
	{
	public:
		PhysicsThread();
		~PhysicsThread();

	public:
		std::thread * launch(std::thread * value);

		bool cleanup() override;
		
		bool wait();

		void sleep(const Duration & value) const;

	public:
		template <
			class Fun,
			class ... Args,
			class = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Fun>>, std::thread>>
		> inline std::thread * launch(Fun && fun, Args && ... args)
		{
			return launch((m_thread)
				? (NULL)
				: (new std::thread(fun, (args)...)));
		}

	public:
		inline void lock() { return m_lock.lock(); }
		inline void unlock() { return m_lock.unlock(); }

	private:
		std::thread *	m_thread;
		MutexLock		m_lock;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_PHYSICS_THREAD_HPP_