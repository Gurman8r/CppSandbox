#include <MemeEditor/ResourceManager.hpp>

namespace ml
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager() 
	{
		cleanup();
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
}