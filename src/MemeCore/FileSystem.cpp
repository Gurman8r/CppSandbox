#include <MemeCore/FileSystem.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/Debug.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <direct.h>
#include <../thirdparty/include/dirent.h>
#else
#include <dirent.h>
#endif

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

	bool FileSystem::setWorkingDir(const String & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		if (_chdir(value.c_str()) == EXIT_SUCCESS)
		{
			ML_EventSystem.fireEvent(FileSystemEvent());
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
		if(DIR * dir = opendir(dirName.c_str()))
		{
			while (dirent * e = readdir(dir))
			{
				switch (e->d_type)
				{
				case DT_REG:
					value << e->d_name << "";
					break;
				case DT_DIR:
					value << e->d_name << "/";
					break;
				case DT_LNK:
					value << e->d_name << "@";
					break;
				default:
					value << e->d_name << "*";
					break;
				}
				value << ml::endl;
			}
			closedir(dir);
			return true;
		}
		value.str(String());
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, List<String> & value) const
	{
		value.clear();
		if (DIR * dir = opendir(dirName.c_str()))
		{
			while (dirent * e = readdir(dir))
			{
				switch (e->d_type)
				{
				case DT_REG:
					value.push_back(String(e->d_name) + "");
					break;
				case DT_DIR:
					value.push_back(String(e->d_name) + "/");
					break;
				case DT_LNK:
					value.push_back(String(e->d_name) + "@");
					break;
				default:
					value.push_back(String(e->d_name) + "*");
					break;
				}
			}
			closedir(dir);
			return true;
		}
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, HashMap<char, List<String>> & value) const
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
				value[type].push_back(String(e->d_name) + type);
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
		if (file.loadFromFile(filename))
		{
			value = file.data();
			return true;
		}
		return false;
	}

	bool FileSystem::getFileContents(const String & filename, String & value) const
	{
		static File file;
		if (file.loadFromFile(filename))
		{
			value = file.to_str();
			return true;
		}
		value.clear();
		return false;
	}

	bool FileSystem::getFileContents(const String & filename, SStream & value) const
	{
		static String temp;
		if (getFileContents(filename, temp))
		{
			value.str(temp);
			return true;
		}
		value.str(String());
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool FileSystem::fileExists(const String & filename) const
	{
		return (bool)(std::ifstream(filename));
	}
	
	String FileSystem::getFileName(const String & filename) const
	{
		size_t i;
		if ((i = filename.find_last_of('/')) != String::npos)
		{
			return filename.substr(i, filename.size() - i);
		}
		return filename;
	}

	String FileSystem::getFileExtension(const String & filename) const
	{
		size_t i;
		if ((i = filename.find_last_of('.')) != String::npos)
		{
			return filename.substr(i, filename.size() - i);
		}
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}