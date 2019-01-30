#include <MemeGraphics/Mesh.h>
#include <MemeCore/FileSystem.h>
#include <MemeCore/Debug.h>

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
		cleanup();
	}

	
	bool Mesh::cleanup()
	{
		return false;
	}
	
	bool Mesh::loadFromFile(const String & filename)
	{
		std::stringstream file;
		if (ML_FileSystem.getFileContents(filename, file))
		{
			file >> (*this);
			return true;
		}
		return false;
	}


	void Mesh::serialize(std::ostream & out) const
	{
		out << "{"
			<< "(VP: " << m_vp.size() << ") " // << std::Endl << m_vp << std::Endl
			<< "(VT: " << m_vt.size() << ") " // << std::Endl << m_vt << std::Endl
			<< "(VN: " << m_vn.size() << ") " // << std::Endl << m_vn << std::Endl
			<< "(VF: " << m_vf.size() << ") " // << std::Endl << m_vi << std::Endl
			<< "}" << std::endl;
	}

	void Mesh::deserialize(std::istream & in)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto parseLine = [](
			const String & line,
			const String & find,
			std::stringstream & data)
		{
			size_t i;
			if ((i = line.find(find)) != String::npos)
			{
				data.str(line.substr((i + find.size()), (line.size() - find.size() - 1)));
				return true;
			}
			return false;
		};
		
		/* * * * * * * * * * * * * * * * * * * * */

		m_vp.clear();
		m_vt.clear();
		m_vn.clear();
		m_vf.clear();

		String line;
		while (std::getline(in, line))
		{
			std::stringstream data;
			if (parseLine(line, "v ", data))
			{
				// Position
				vec3f temp;
				data >> temp;
				m_vp.push_back(temp);
			}
			else if (parseLine(line, "vt ", data))
			{
				// Texcoord
				vec2f temp;
				data >> temp;
				m_vt.push_back(temp);
			}
			else if (parseLine(line, "vn ", data))
			{
				// Normal
				vec3f temp;
				data >> temp;
				m_vn.push_back(temp);
			}
			else if (parseLine(line, "f ", data))
			{
				// Index
				String temp;
				while (std::getline(data, temp, '/'))
				{
					m_vf.push_back(std::stoi(temp));
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		// TODO...

		/* * * * * * * * * * * * * * * * * * * * */
	}

}