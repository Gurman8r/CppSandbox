#include <MemeGraphics/Material.hpp>
#include <MemeCore/File.hpp>

namespace ml
{
	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	bool Material::cleanup()
	{
		return true;
	}

	bool Material::loadFromFile(const String & filename)
	{
		static File file;
		if (file.loadFromFile(filename))
		{
			return true;
		}
		return false;
	}


	void Material::serialize(std::ostream & out) const
	{
	}

	void Material::deserialize(std::istream & in)
	{
	}
}