#include <MemeCore/FileSystem.hpp>
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

	bool FileSystem::setWorkingDir(const String & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		return (_chdir(value.c_str()) == EXIT_SUCCESS);
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
		static String::Stream temp;
		if (getDirContents(dirName, temp))
		{
			value.assign(temp.str());
			return true;
		}
		value.clear();
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, String::Stream & value) const
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


	/* * * * * * * * * * * * * * * * * * * * */

	bool FileSystem::fileExists(const String & filename) const
	{
		return (bool)(std::ifstream(filename));
	}


	bool FileSystem::getFileContents(const String & filename, List<char> & value) const
	{
		value.clear();

		if (!filename.empty())
		{
			std::ifstream file(filename.c_str(), std::ios_base::binary);
			if (file)
			{
				file.seekg(0, std::ios_base::end);

				std::streamsize size;
				if ((size = file.tellg()) > 0)
				{
					file.seekg(0, std::ios_base::beg);

					value.resize(static_cast<size_t>(size));

					file.read(&value[0], size);
				}

				value.push_back('\0');

				file.close();

				return true;
			}
			else
			{
				return Debug::logError("FileSystem: File not found \'{0}\'", filename);
			}
		}
		else
		{
			return Debug::logError("FileSystem: Filename cannot be empty");
		}
	}

	bool FileSystem::getFileContents(const String & filename, String & value) const
	{
		static List<char> temp;
		if (getFileContents(filename, temp))
		{
			value.assign(temp.begin(), temp.end());
			return true;
		}
		value.clear();
		return false;
	}

	bool FileSystem::getFileContents(const String & filename, String::Stream & value) const
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
	
	String FileSystem::getFileExtension(const String & filename) const
	{
		if (fileExists(filename))
		{
			size_t i;
			if ((i = filename.find_last_of('.')) != String::npos)
			{
				return filename.substr(i, filename.size() - i);
			}
		}
		return String();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}