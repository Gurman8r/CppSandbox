#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <MemeCore/IReadable.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeEngine/LibLoader.hpp>
#include <MemeEngine/PluginAPI.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API SharedLibrary final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public INonCopyable
	{
	public:
		SharedLibrary();
		~SharedLibrary();

	public:
		bool	dispose() override;
		bool	loadFromFile(const String & filename) override;
		void *	loadFunction(const String & name);

	public:
		template <
			class Res,
			class ... Args
		> inline Res callFunction(const String & name, Args && ... args)
		{
			using Fun = Res(*)(Args...);
			static Fun fun = NULL;
			return ((fun = (Fun)(loadFunction(name)))
				? ((Res)(fun((args)...)))
				: (Res()));
		}

	private:
		void * m_instance; // Instance
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_