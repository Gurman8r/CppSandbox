#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeCore/File.hpp>
#include <MemeCore/List.hpp>

#define ML_FileSystem ml::FileSystem::getInstance()

namespace ml
{
	class ML_CORE_API FileSystem final
		: public ITrackable
		, public ISingleton<FileSystem>
	{
		friend class ISingleton<FileSystem>;

	private:
		FileSystem();
		~FileSystem();

		String m_root;

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		inline const String & root() const 
		{ 
			return m_root; 
		}

		inline const String pathTo(const String & value) const
		{
			return root() + "\\" + value;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		bool	setWorkingDir(const String & value);
		String	getWorkingDir() const;

		bool	getDirContents(const String & dirName, List<char> & value) const;
		bool	getDirContents(const String & dirName, String & value) const;
		bool	getDirContents(const String & dirName, SStream & value) const;
		bool	getDirContents(const String & dirName, HashMap<char, List<String>> & value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool	getFileContents(const String & filename, List<char> & value) const;
		bool	getFileContents(const String & filename, String & value) const;
		bool	getFileContents(const String & filename, SStream & value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool	dirExists(const String & dirName) const;
		bool	fileExists(const String & filename) const;
		String	getFileExt(const String & filename) const;
		String  getFileName(const String & filename) const;
		size_t	getFileSize(const String & filename) const;

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_FILE_SYSTEM_HPP_