#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <MemeCore/IReadable.hpp>
#include <MemeEngine/LibLoader.hpp>
#include <MemeEngine/PluginAPI.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Plugin final
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

	public:
		inline const String & file() const { return m_file; }

	private:
		void * m_inst; // Instance
		String m_file; // File Name
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_