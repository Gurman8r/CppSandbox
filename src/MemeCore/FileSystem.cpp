#include <MemeCore/FileSystem.h>
#include <MemeCore/DebugUtility.h>
#include <fstream>

#ifdef ML_SYSTEM_WINDOWS
#include <direct.h>
#include <dirent.h>
#endif


namespace ml
{
	bool FileSystem::changeDir(const std::string & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		return (_chdir(value.c_str()) == EXIT_SUCCESS) ? true : false;
#else
		return false;
#endif
	}

	bool FileSystem::fileExists(const std::string & filename)
	{
		return (bool)(std::ifstream(filename));
	}

	bool FileSystem::getFileContents(const std::string & filename, std::vector<char> & buffer)
	{
		buffer.clear();
		if (!filename.empty())
		{
			std::ifstream file(filename.c_str(), std::ios_base::binary);
			if (file)
			{
				file.seekg(0, std::ios_base::end);

				std::streamsize size = file.tellg();

				if (size > 0)
				{
					file.seekg(0, std::ios_base::beg);

					buffer.resize(static_cast<std::size_t>(size));

					file.read(&buffer[0], size);
				}

				buffer.push_back('\0');

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

	bool FileSystem::getFileContents(const std::string & filename, std::string & str)
	{
		static std::vector<char> buffer;
		str.clear();
		if (getFileContents(filename, buffer))
		{
			str = std::string(buffer.begin(), buffer.end());
			return true;
		}
		return false;
	}

	bool FileSystem::getFileContents(const std::string & filename, std::stringstream & stream)
	{
		static std::string buffer;
		stream.str(std::string());
		if (getFileContents(filename, buffer))
		{
			stream.str(buffer);
			return true;
		}
		return false;
	}
	
}