#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <MemeCore/LibLoader.hpp>
#include <MemeCore/IReadable.hpp>

#define ML_PluginMain ML_PluginMain

extern "C"
{
	ML_API_EXPORT void ML_PluginMain(void * data);
}

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

		bool	cleanup() override;
		bool	loadFromFile(const String & filename) override;
		bool	call(const String & func, void * data);
		bool	main(void * data);

	private:
		String		m_name; // File Name
		void *		m_inst; // Instance Handle
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_