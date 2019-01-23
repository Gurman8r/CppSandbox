#include <MemeGraphics/Mesh.h>
#include <MemeCore/FileSystem.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const Mesh & copy)
	{
	}

	Mesh::~Mesh()
	{
	}

	
	bool Mesh::cleanup()
	{
		return false;
	}
	
	bool Mesh::loadFromFile(const std::string & filename)
	{
		auto parseLine = [](
			const std::string & line,
			const std::string & find,
			std::stringstream & data)
		{
			std::size_t i;
			if ((i = line.find(find)) != std::string::npos)
			{
				data.str(line.substr((i + find.size()), (line.size() - find.size() - 1)));
				return true;
			}
			data.str(std::string());
			return false;
		};

		std::stringstream file;
		if (ML_FileSystem.getFileContents(filename, file))
		{
			std::string line;
			while (std::getline(file, line))
			{
				std::stringstream data;
				if (parseLine(line, "v ", data)) // Position
				{
					vec3f tmp;
					data >> tmp;

					Debug::out() << "Position: " << tmp << std::endl;
				}
				else if (parseLine(line, "vt ", data)) // Texcoord
				{
					vec2f tmp;
					data >> tmp;

					Debug::out() << "Texcoord: " << tmp << std::endl;
				}
				else if (parseLine(line, "vn ", data)) // Normal
				{
					vec3f tmp;
					data >> tmp;

					Debug::out() << "Normal: " << tmp << std::endl;
				}
				else if (parseLine(line, "f ", data)) // Index
				{
					IndexList tmp;
					std::string i;
					while (std::getline(data, i, '/'))
					{
						tmp.push_back(std::stoi(i));
					}

					Debug::out() << "Index: " << tmp << std::endl;
				}
			}
			return true;
		}
		return false;
	}


	void Mesh::serialize(std::ostream & out) const
	{
	}

	void Mesh::deserialize(std::istream & in)
	{
	}

}