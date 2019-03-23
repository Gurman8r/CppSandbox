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
		bool call_main(void * data);

	private:
		String		m_name; // File Name
		void *		m_inst; // Instance Handle
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_