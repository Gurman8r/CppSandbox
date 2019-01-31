#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/String.h>

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
		bool	fileExists(const string & filename) const;
		bool	getFileContents(const string & filename, std::vector<char> & buffer) const;
		bool	getFileContents(const string & filename, string & str) const;
		bool	getFileContents(const string & filename, std::stringstream & stream) const;

		bool	setWorkingDir(const string & value);
		string	getWorkingDir() const;

		string	getFileExtension(const string & filename) const;
	};
}

#endif // !_FILE_SYSTEM_H_
