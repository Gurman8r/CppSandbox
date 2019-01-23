#include <MemeGraphics/Mesh.h>
#include <MemeCore/FileSystem.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	Mesh::Mesh()
		: m_requiresUpdate(true)
		, m_vertices()
		, m_indices()
		, m_textures()
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_requiresUpdate(copy.m_requiresUpdate)
		, m_vertices()
		, m_indices()
		, m_textures()
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

		bool displayAll = false;

		std::stringstream file;
		if (ML_FileSystem.getFileContents(filename, file))
		{
			std::string line;
			while (std::getline(file, line))
			{
				std::stringstream data;
				if (parseLine(line, "v ", data)) // Vertex
				{
					vec3f tmp;
					data >> tmp;

					if (displayAll)
						Debug::out() << "Vertex: " << tmp << std::endl;
				}
				else if (parseLine(line, "vt ", data)) // Texcoord
				{
					vec2f tmp;
					data >> tmp;

					if (displayAll)
						Debug::out() << "Texcoord: " << tmp << std::endl;
				}
				else if (parseLine(line, "vn ", data)) // Normal
				{
					vec3f tmp;
					data >> tmp;
					
					if (displayAll)
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

					if (displayAll)
						Debug::out() << "Index: " << tmp << std::endl;
				}
			}
			return true;
		}
		return false;
	}

	
	Mesh & Mesh::vertices(const VertexList & value)
	{
		if (m_vertices != value)
		{
			m_requiresUpdate = true;
			m_vertices = value;
		}
		return (*this);
	}

	Mesh & Mesh::indices(const IndexList & value)
	{
		if(m_indices != value)
		{
			m_requiresUpdate = true;
			m_indices = value;
		}
		return (*this);
	}

	Mesh & Mesh::textures(const TextureList & value)
	{
		if(m_textures != value)
		{
			m_requiresUpdate = true;
			m_textures = value;
		}
		return (*this);
	}


	void Mesh::serialize(std::ostream & out) const
	{
		for (auto v : vertices())
		{

		}

		for (auto i : indices())
		{

		}

		for (auto t : textures())
		{

		}
	}

	void Mesh::deserialize(std::istream & in)
	{
	}

}