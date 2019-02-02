#include <MemeGraphics/Mesh.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	Mesh::Mesh()
		: m_vertices()
		, m_texcoords()
		, m_normals()
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_vertices(copy.m_vertices)
		, m_texcoords(copy.m_texcoords)
		, m_normals(copy.m_normals)
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
		String::Stream file;
		if (ML_FileSystem.getFileContents(filename, file))
		{
			file >> (*this);
			return true;
		}
		return false;
	}


	void Mesh::serialize(std::ostream & out) const
	{
		out << "Mesh:"		<< endl
			<< m_vertices	<< endl
			<< m_texcoords	<< endl
			<< m_normals	<< endl
			<< endl;
	}

	void Mesh::deserialize(std::istream & in)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto parseLine = [](
			const String & line,
			const String & find,
			String::Stream & ss)
		{
			size_t i;
			if ((i = line.find(find)) != String::npos)
			{
				ss.str(line.substr((i + find.size()), (line.size() - find.size() - 1)));
				return true;
			}
			ss.str(String());
			return false;
		};
		
		/* * * * * * * * * * * * * * * * * * * * */

		List<vec3f>		vp; // Vertices
		List<vec2f>		vt; // Texcoords
		List<vec3f>		vn; // Normals
		List<IndexList> vf;	// Faces

		/* * * * * * * * * * * * * * * * * * * * */

		String line;
		while (std::getline(in, line))
		{
			if (line.front() == '#')
				continue;

			String::Stream ss;
			if (parseLine(line, "v ", ss))
			{
				// Vertex
				vec3f temp;
				ss >> temp;
				vp.push_back(temp);
			}
			else if (parseLine(line, "vt ", ss))
			{
				// Texcoord
				vec2f temp;
				ss >> temp;
				vt.push_back(temp);
			}
			else if (parseLine(line, "vn ", ss))
			{
				// Normal
				vec3f temp;
				ss >> temp;
				vn.push_back(temp);
			}
			else if (parseLine(line, "f ", ss))
			{
				// Face
				vf.push_back(IndexList());

				String::Stream lineStream(ss.str());
				while (lineStream.good())
				{
					lineStream >> line;

					String::Stream indexStream(line);
					while (std::getline(indexStream, line, '/'))
					{
						vf.back().push_back(std::stoi(line));
					}
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		IndexList vi; // Vertex Indices
		IndexList ti; // Texcoord Indices
		IndexList ni; // Normal Indices

		for (size_t i = 0, imax = vf.size(); i < imax; i++)
		{
			vi.push_back(vf[i][0] - 1);
			ti.push_back(vf[i][1] - 1);
			ni.push_back(vf[i][2] - 1);

			vi.push_back(vf[i][3] - 1);
			ti.push_back(vf[i][4] - 1);
			ni.push_back(vf[i][5] - 1);

			vi.push_back(vf[i][6] - 1);
			ti.push_back(vf[i][7] - 1);
			ni.push_back(vf[i][8] - 1);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		m_vertices.clear();
		m_texcoords.clear();
		m_normals.clear();

		for (const uint32_t i : vi)
		{
			m_vertices.push_back(vp[i]);
		}

		for (const uint32_t i : ti)
		{
			m_texcoords.push_back(vt[i]);
		}

		for (const uint32_t i : ni)
		{
			m_normals.push_back(vn[i]);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

}