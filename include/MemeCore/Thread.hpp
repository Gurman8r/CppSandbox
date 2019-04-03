#ifndef _ML_THREAD_HPP_
#define _ML_THREAD_HPP_

#include <MemeCore/Duration.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeCore/Lock.hpp>

#define ML_THREAD_CONDITIONS(Fun) \
std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Fun>>, std::thread>>

namespace ml
{
	class ML_CORE_API Thread final
		: public ITrackable
		, public IDisposable
		, public INonCopyable
	{
	public:
		Thread();
		~Thread();

	public:
		template <
			class Fun, 
			class ... Args, 
			class = ML_THREAD_CONDITIONS(Fun)
		> inline std::thread * launch(Fun && fun, Args && ... args)
		{
			return alive() ? (NULL) : (m_thr = new std::thread(fun, (args)...));
		}

		bool alive() const;
		bool joinable() const;
		bool cleanup() override;
		void sleep(const Duration & value);

	private:
		std::thread * m_thr;
	};
}

#endif // !_ML_THREAD_HPP_