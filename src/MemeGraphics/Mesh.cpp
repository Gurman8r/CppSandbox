#include <MemeGraphics/Mesh.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

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
		out << "Mesh:" << endl

			<< "V: " << m_vertices.size() << endl
			<< m_vertices << endl

			<< "I: " << m_indices.size() << endl
			<< m_indices << endl
			
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
			return false;
		};
		
		// Read File
		/* * * * * * * * * * * * * * * * * * * * */
		List<vec3f>		vp; // Vertices
		List<vec2f>		vt; // Texcoords
		List<vec3f>		vn; // Normals
		List<IndexList> vf;	// Faces

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

		// Indices
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
			ni.push_back(vf[i][8] - 1);
			ti.push_back(vf[i][7] - 1);
		}

		if ((vi.size() != ti.size()) || (vi.size() != ni.size()))
		{
			Debug::logError("Mesh: Index List Size Mismatch");
			return;
		}


		// Vertices
		/* * * * * * * * * * * * * * * * * * * * */
		m_vertices.resize(vi.size());

		for (size_t i = 0, imax = m_vertices.size(); i < imax; i++)
		{
			m_vertices[i] = Vertex(
				vp[vi[i]],
				Color::White, //vec4f(vn[ni[i]], 1),
				vt[ti[i]]);
		}

		m_contiguous = m_vertices.contiguous();

		/* * * * * * * * * * * * * * * * * * * * */
	}

}