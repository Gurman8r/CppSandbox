#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <MemeCore/Export.h>
#include <string>
#include <vector>

namespace ml
{
	class ML_CORE_API FileSystem final
	{
	public:
		static bool fileExists(const std::string & filename);
		static bool getFileContents(const std::string& filename, std::vector<char>& buffer);
	};
}

#endif // !_FILE_SYSTEM_H_
