#include <MemeCore/FileSystem.h>
#include <MemeCore/DebugUtility.h>
#include <fstream>

#ifdef ML_SYSTEM_WINDOWS
#include <direct.h>
#include <dirent.h>
#endif // ML_SYSTEM_WINDOWS


namespace ml
{
	bool FileSystem::changeDir(const std::string & value)
	{
		return (_chdir(value.c_str()) == EXIT_SUCCESS) ? true : false;
	}

	bool FileSystem::fileExists(const std::string & filename)
	{
		return (bool)(std::ifstream(filename));
	}

	bool FileSystem::getFileContents(const std::string & filename, std::vector<char>& buffer)
	{
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
				return Debug::LogError("FS: File not found \'{0}\'", filename);
			}
		}
		else
		{
			return Debug::LogError("FS: Filename cannot be empty");
		}
	}
}