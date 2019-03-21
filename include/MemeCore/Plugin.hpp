#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <MemeCore/LibLoader.hpp>
#include <MemeCore/IReadable.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Plugin_Main ML_Plugin_Main

extern "C"
{
	ML_API_EXPORT int32_t ML_Plugin_Main(void * user_data);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Plugin
		: public ITrackable
		, public IReadable
		, public INonCopyable
	{
	public:
		using PluginFun = int32_t (*)(void *);

	public:
		Plugin();
		~Plugin();

		bool	cleanup() override;
		bool	loadFromFile(const String & filename) override;
		int32_t entryPoint(void * user_data);

	public:
		template <typename T>
		inline int32_t entryPoint(T user_data)
		{
			return entryPoint((void *)(user_data));
		}

	private:
		String		m_name; // File Name
		void *		m_inst; // Instance Handle
		PluginFun	m_main; // Entry Point
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_