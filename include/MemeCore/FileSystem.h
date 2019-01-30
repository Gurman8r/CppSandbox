#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/String.h>
#include <sstream>
#include <vector>

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
		bool	fileExists(const String & filename);
		bool	getFileContents(const String & filename, std::vector<char> & buffer);
		bool	getFileContents(const String & filename, String & str);
		bool	getFileContents(const String & filename, std::stringstream & stream);

		bool		setWorkingDir(const String & value);
		String getWorkingDir() const;
	};
}

#endif // !_FILE_SYSTEM_H_
