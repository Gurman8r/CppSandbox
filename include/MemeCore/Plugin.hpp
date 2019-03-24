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
		using PluginFun = void(*)(void *);

	public:
		Plugin();
		~Plugin();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool call(const String & name, void * data);

	public:
		template <
			typename T
		> inline bool call(const String & name, T data)
		{
			return call(name, (void *)(data));
		}

		template <
			typename T
		> inline bool init(T data) 
		{ 
			return call<T>(ML_STRINGIFY(ML_PluginInit), data);
		}

		template <
			typename T
		> inline bool enable(T data)
		{
			return call<T>(ML_STRINGIFY(ML_PluginEnable), data);
		}

		template <
			typename T
		> inline bool disable(T data)
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