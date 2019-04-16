#ifndef _ML_PREFERENCES_HPP_
#define _ML_PREFERENCES_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IReadable.hpp>

#define ML_Prefs ml::Preferences::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Meant to replace Demo Settings
	// WIP
	class ML_ENGINE_API Preferences final
		: public ITrackable
		, public IReadable
		, public ISingleton<Preferences>
	{
		friend class ISingleton<Preferences>;

	private:
		Preferences();
		~Preferences();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

	public:
		bool	GetBoolean	(String section, String name, bool default_value);
		float	GetFloat	(String section, String name, float default_value);
		long	GetInteger	(String section, String name, long default_value);
		String	GetString	(String section, String name, String default_value);
	
	private:
		void * m_ini;
	};
}

#endif // !_ML_PREFERENCES_HPP_