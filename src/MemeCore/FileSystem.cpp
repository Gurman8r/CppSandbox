#include <MemeCore/FileSystem.h>
#include <MemeCore/Debug.h>
#include <fstream>

#ifdef ML_SYSTEM_WINDOWS
#include <direct.h>
#endif


namespace ml
{
	bool FileSystem::fileExists(const String & filename) const
	{
		return (bool)(std::ifstream(filename));
	}

	bool FileSystem::getFileContents(const String & filename, std::vector<char> & buffer) const
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
		static std::vector<char> buffer;
		str.clear();
		if (getFileContents(filename, buffer))
		{
			str = String(buffer.begin(), buffer.end());
			return true;
		}
		return false;
	}

	bool FileSystem::getFileContents(const String & filename, std::stringstream & stream) const
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