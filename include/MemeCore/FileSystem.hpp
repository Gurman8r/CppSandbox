#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <MemeCore/File.hpp>

#define ML_FS ml::FileSystem::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API FileSystem final
		: public ITrackable
		, public ISingleton<FileSystem>
	{
		friend class ISingleton<FileSystem>;

	public:
		using Directory = typename HashMap<char, List<String>>;

	private:
		FileSystem();
		~FileSystem();

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		const String &	getRoot() const;
		const String	getPathTo(const String & value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool	setWorkingDir(const String & value);
		String	getWorkingDir() const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool	getDirContents(const String & dirName, List<char> & value) const;
		bool	getDirContents(const String & dirName, String & value) const;
		bool	getDirContents(const String & dirName, SStream & value) const;
		bool	getDirContents(const String & dirName, Directory & value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool	getFileContents(const String & filename, List<char> & value) const;
		bool	getFileContents(const String & filename, String & value) const;
		bool	getFileContents(const String & filename, SStream & value) const;

		/* * * * * * * * * * * * * * * * * * * * */

		bool	dirExists(const String & dirName) const;
		bool	fileExists(const String & filename) const;

		/* * * * * * * * * * * * * * * * * * * * */

		String	getFileType(const String & filename) const;
		String  getFileName(const String & filename) const;
		String	getFilePath(const String & filename) const;
		size_t	getFileSize(const String & filename) const;

		/* * * * * * * * * * * * * * * * * * * * */

	private:
		String m_root;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_