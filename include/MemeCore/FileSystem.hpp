#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <MemeCore/ISingleton.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/String.hpp>

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
		bool	fileExists(const String & filename) const;
		bool	getFileContents(const String & filename, std::vector<char> & buffer) const;
		bool	getFileContents(const String & filename, String & str) const;
		bool	getFileContents(const String & filename, String::Stream & stream) const;

		bool	setWorkingDir(const String & value);
		String	getWorkingDir() const;

		String	getFileExtension(const String & filename) const;
	};
}

#endif // !_FILE_SYSTEM_H_
