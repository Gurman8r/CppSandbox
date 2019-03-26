#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <MemeCore/LibLoader.hpp>
#include <MemeCore/IReadable.hpp>
#include <MemeCore/PluginAPI.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Plugin
		: public ITrackable
		, public IReadable
		, public INonCopyable
	{
	public:
		Plugin();
		~Plugin();

	public:
		bool	cleanup() override;
		bool	loadFromFile(const String & filename) override;
		void *	call(const String & name, void * data);

	public:
		template <
			typename T,
			typename R = void *
		> inline R call(const String & name, T data)
		{
			return call(name, (void *)(data));
		}

		template <
			typename T
		> inline void * init(T data)
		{ 
			return call<T>(ML_STRINGIFY(ML_PluginInit), data);
		}

		template <
			typename T
		> inline void * enable(T data)
		{
			return call<T>(ML_STRINGIFY(ML_PluginEnable), data);
		}

		template <
			typename T
		> inline void * disable(T data)
		{
			return call<T>(ML_STRINGIFY(ML_PluginDisable), data);
		}

	public:
		inline const String & file() const { return m_file; }

	private:
		void * m_inst; // Instance
		String m_file; // File Name
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_