#ifndef _FILE_SYSTEM_HPP_
#define _FILE_SYSTEM_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeCore/Directory.hpp>
#include <MemeCore/List.hpp>

#define ML_FileSystem ml::FileSystem::getInstance()

namespace ml
{
	class ML_CORE_API FileSystem final
		: public ITrackable
		, public ISingleton<FileSystem>
	{
		friend class ISingleton<FileSystem>;

	private:
		FileSystem() {}
		~FileSystem() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		bool	setWorkingDir(const String & value);
		String	getWorkingDir() const;
		bool	getDirContents(const String & dirName, List<char> & buffer) const;
		bool	getDirContents(const String & dirName, String & str) const;
		bool	getDirContents(const String & dirName, String::Stream & ss) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool	fileExists(const String & filename) const;
		bool	getFileContents(const String & filename, List<char> & buffer) const;
		bool	getFileContents(const String & filename, String & str) const;
		bool	getFileContents(const String & filename, String::Stream & ss) const;
		String	getFileExtension(const String & filename) const;

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_FILE_SYSTEM_HPP_
