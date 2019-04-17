#ifndef _ML_LUA_SCRIPT_HPP_
#define _ML_LUA_SCRIPT_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API LuaScript
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
	public:
		LuaScript();
		~LuaScript();

	public:
		virtual bool dispose() override;
		virtual bool loadFromFile(const String & filename) override;

	private:
		File m_file;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LUA_SCRIPT_HPP_