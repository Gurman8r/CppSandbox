#include <MemeCore/FileSystem.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/Debug.hpp>

#include <experimental/filesystem>

# ifdef ML_SYSTEM_WINDOWS
#	include <dirent.h> // from /thirdparty/include
#	include <direct.h>
#	include <sys/types.h>
#	include <sys/stat.h>
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	FileSystem::FileSystem()
		: m_root(getWorkingDir())
	{
	}

	FileSystem::~FileSystem()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const String & FileSystem::getRoot() const
	{
		return m_root;
	}

	const String FileSystem::getPathTo(const String & value) const
	{
#ifdef ML_SYSTEM_WINDOWS
		return (m_root + "\\" + value);
#else
		return (m_root + "/" + value);
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool FileSystem::setWorkingDir(const String & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		if (_chdir(value.c_str()) == EXIT_SUCCESS)
		{
			ML_EventSystem.fireEvent(FS_ChangDirEvent(getWorkingDir()));
			return true;
		}
		return false;
#else
		return false;
#endif
	}

	String FileSystem::getWorkingDir() const
	{
#ifdef ML_SYSTEM_WINDOWS
		return String(_getcwd(NULL, 0));
#else
		return String();
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool FileSystem::getDirContents(const String & dirName, List<char> & value) const
	{
		static String temp;
		if (getDirContents(dirName, temp))
		{
			value.assign(temp.begin(), temp.end());
			return true;
		}
		value.clear();
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, String & value) const
	{
		static SStream temp;
		if (getDirContents(dirName, temp))
		{
			value.assign(temp.str());
			return true;
		}
		value.clear();
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, SStream & value) const
	{
		value.str(String());
		if(DIR * dir = opendir(dirName.c_str()))
		{
			while (dirent * e = readdir(dir))
			{
				switch (e->d_type)
				{
				case DT_DIR	: value << e->d_name << "/" << endl; break;
				default		: value << e->d_name << " " << endl; break;
				}
			}
			closedir(dir);
			return true;
		}
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, Directory & value) const
	{
		value.clear();
		if (DIR * dir = opendir(dirName.c_str()))
		{
			while (dirent * e = readdir(dir))
			{
				char type;
				switch (e->d_type)
				{
				case DT_DIR: (type = '/'); break;
				case DT_REG: (type = ' '); break;
				case DT_LNK: (type = '@'); break;
				default:	 (type = '*'); break;
				}
				value[type].push_back(e->d_name);
			}
			closedir(dir);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool FileSystem::getFileContents(const String & filename, List<char> & value) const
	{
		static File file;
		file.loadFromFile(filename);
		value = file.data();
		return file;
	}

	bool FileSystem::getFileContents(const String & filename, String & value) const
	{
		static File file;
		file.loadFromFile(filename);
		value = file.ToString();
		return file;
	}

	bool FileSystem::getFileContents(const String & filename, SStream & value) const
	{
		static File file;
		file.loadFromFile(filename);
		value = file.ToStream();
		return file;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool FileSystem::dirExists(const String & name) const
	{
		struct stat info;
		return 
			(stat(name.c_str(), &info) == EXIT_SUCCESS) && 
			(info.st_mode & S_IFDIR);
	}

	bool FileSystem::fileExists(const String & filename) const
	{
		return (bool)(std::ifstream(filename));
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	String FileSystem::getFileType(const String & filename) const
	{
		size_t i;
		return (
			(
			((i = filename.find_last_of('.')) != String::npos) ||
			((i = filename.find_last_of('/')) != String::npos) ||
			((i = filename.find_last_of('\\')) != String::npos)
			)
			? (String(filename.substr(i + 1, filename.size() - i - 1)))
			: (String())
		);
	}

	String FileSystem::getFileName(const String & filename) const
	{
		size_t i;
		return (
			(
			((i = filename.find_last_of('/')) != String::npos) ||
			((i = filename.find_last_of('\\')) != String::npos)
			)
			? (String(filename.substr(i + 1, filename.size() - i - 1)))
			: (filename)
		);
	}

	String FileSystem::getFilePath(const String & filename) const
	{
		size_t i;
		return (
			(
			((i = filename.find_last_of('/')) != String::npos) ||
			((i = filename.find_last_of('\\')) != String::npos)
			)
			? (String(filename.substr(0, i)))
			: (filename)
		);
	}

	size_t FileSystem::getFileSize(const String & filename) const
	{
		std::ifstream stream;
		return (
			(stream = std::ifstream(filename, std::ifstream::ate | std::ifstream::binary))
				? ((size_t)(stream.tellg()))
				: ((size_t)(0))
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}