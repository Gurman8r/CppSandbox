#include <MemeEditor/ResourceManager.hpp>

namespace ml
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager() 
	{
	}

	void ResourceManager::cleanup()
	{
		sounds.clear();
		sprites.clear();
		models.clear();
		meshes.clear();
		shaders.clear();
		textures.clear();
		images.clear();
		fonts.clear();
	}

	bool ResourceManager::loadManifest(const Manifest & value)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		for (auto pair : value.getTypes())
		{
			const String & type = pair.first;

			if (type == "font")
			{
				this->fonts.load(value.path(), pair.second);
			}
			else if (type == "image")
			{
				this->images.load(value.path(), pair.second);
			}
			else if (type == "mesh")
			{
				this->meshes.load(value.path(), pair.second);
			}
			else if (type == "model")
			{
				this->models.load(value.path(), pair.second);
			}
			else if (type == "shader")
			{
				this->shaders.load(value.path(), pair.second);
			}
			else if (type == "sound")
			{
				this->sounds.load(value.path(), pair.second);
			}
			else if (type == "sprite")
			{
				this->sprites.load(value.path(), pair.second);
			}
			else if (type == "texture")
			{
				this->textures.load(value.path(), pair.second);
			}
		}

		return true;

		/* * * * * * * * * * * * * * * * * * * * */
	}
}