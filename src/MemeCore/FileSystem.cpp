#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <direct.h>
#include <../thirdparty/include/dirent.h>
#endif

namespace ml
{
	bool FileSystem::getDirContents(const String & dirName, List<char> & buffer) const
	{
		static String str;
		if (getDirContents(dirName, str))
		{
			buffer = List<char>(str.begin(), str.end());
			return true;
		}
		buffer.clear();
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, String & str) const
	{
		static String::Stream stream;
		if (getDirContents(dirName, stream))
		{
			str = stream.str();
			return true;
		}
		str = String();
		return false;
	}

	bool FileSystem::getDirContents(const String & dirName, String::Stream & stream) const
	{
		if(DIR * dir = opendir(dirName.c_str()))
		{
			while (dirent * e = readdir(dir))
			{
				switch (e->d_type)
				{
				case DT_REG:
					stream << e->d_name << "";
					break;
				case DT_DIR:
					stream << e->d_name << "/";
					break;
				case DT_LNK:
					stream << e->d_name << "@";
					break;
				default:
					stream << e->d_name << "*";
					break;
				}
				stream << ml::endl;
			}
			closedir(dir);
			return true;
		}
		stream.str(String());
		return false;
	}

	bool FileSystem::setWorkingDir(const String & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		return (_chdir(value.c_str()) == EXIT_SUCCESS) ? true : false;
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


	bool FileSystem::fileExists(const String & filename) const
	{
		return (bool)(std::ifstream(filename));
	}

	bool FileSystem::getFileContents(const String & filename, List<char> & buffer) const
	{
		buffer.clear();
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

					buffer.resize(static_cast<size_t>(size));

					file.read(&buffer[0], size);
				}
				buffer.push_back('\0');
				file.close();
				return true;
			}
			else
			{
				return Debug::LogError("FileSystem: File not found \'{0}\'", filename);
			}
		}
		else
		{
			return Debug::LogError("FileSystem: Filename cannot be empty");
		}
	}

	bool FileSystem::getFileContents(const String & filename, String & str) const
	{
		static List<char> buffer;
		str.clear();
		if (getFileContents(filename, buffer))
		{
			str = String(buffer.begin(), buffer.end());
			return true;
		}
		return false;
	}

	bool FileSystem::getFileContents(const String & filename, String::Stream & stream) const
	{
		static String buffer;
		stream.str(String());
		if (getFileContents(filename, buffer))
		{
			stream.str(buffer);
			return true;
		}
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


}