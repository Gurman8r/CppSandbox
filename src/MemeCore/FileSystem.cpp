#include <MemeCore/FileSystem.h>
#include <MemeCore/Debug.h>
#include <fstream>

#ifdef ML_SYSTEM_WINDOWS
#include <direct.h>
#endif


namespace ml
{
	bool FileSystem::fileExists(const string & filename) const
	{
		return (bool)(std::ifstream(filename));
	}

	bool FileSystem::getFileContents(const string & filename, std::vector<char> & buffer) const
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

	bool FileSystem::getFileContents(const string & filename, string & str) const
	{
		static std::vector<char> buffer;
		str.clear();
		if (getFileContents(filename, buffer))
		{
			str = string(buffer.begin(), buffer.end());
			return true;
		}
		return false;
	}

	bool FileSystem::getFileContents(const string & filename, std::stringstream & stream) const
	{
		static string buffer;
		stream.str(string());
		if (getFileContents(filename, buffer))
		{
			stream.str(buffer);
			return true;
		}
		return false;
	}
	

	bool FileSystem::setWorkingDir(const string & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		return (_chdir(value.c_str()) == EXIT_SUCCESS) ? true : false;
#else
		return false;
#endif
	}

	string FileSystem::getWorkingDir() const
	{
#ifdef ML_SYSTEM_WINDOWS
		return string(_getcwd(NULL, 0));
#else
		return string();
#endif
	}


	string FileSystem::getFileExtension(const string & filename) const
	{
		if (fileExists(filename))
		{
			size_t i;
			if ((i = filename.find_last_of('.')) != string::npos)
			{
				return filename.substr(i, filename.size() - i);
			}
		}
		return string();
	}


}