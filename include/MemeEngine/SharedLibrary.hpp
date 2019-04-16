#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IReadable.hpp>
#include <MemeCore/IDisposable.hpp>

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
			static Fun fun;
			return ((fun = (Fun)(loadFunction(name)))
				? ((Res)(fun((args)...)))
				: (Res()));
		}

	public:
		inline const String &	filename() const { return m_filename; }
		inline const void *		instance() const { return m_instance; }

	private:
		String m_filename;
		void * m_instance;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_