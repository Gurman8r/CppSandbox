#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/ITrackable.h>
#include <string>
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
		bool	fileExists(const std::string & filename);
		bool	getFileContents(const std::string & filename, std::vector<char> & buffer);
		bool	getFileContents(const std::string & filename, std::string & str);
		bool	getFileContents(const std::string & filename, std::stringstream & stream);

		bool		setWorkingDir(const std::string & value);
		std::string getWorkingDir() const;
	};
}

#endif // !_FILE_SYSTEM_H_
