#include <MemeEngine/SharedLibrary.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SharedLibrary::SharedLibrary()
		: m_instance(NULL)
		, m_filename()
	{
	}

	SharedLibrary::~SharedLibrary()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SharedLibrary::dispose()
	{
#ifdef ML_SYSTEM_WINDOWS
		return (FreeLibrary((HINSTANCE)m_instance));
#else
		return false;
#endif
	}

	bool SharedLibrary::loadFromFile(const String & filename)
	{
#ifdef ML_SYSTEM_WINDOWS
		return (m_instance = LoadLibraryA((m_filename = filename).c_str()));
#else
		return false;
#endif
	}

	void * SharedLibrary::loadFunction(const String & name)
	{
#ifdef ML_SYSTEM_WINDOWS
		return ((m_instance)
			? (GetProcAddress((HINSTANCE)m_instance, name.c_str()))
			: (NULL));
#else
		return NULL;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * */
}