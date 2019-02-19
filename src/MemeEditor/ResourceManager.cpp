#include <MemeEditor/ResourceManager.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager() 
	{
		cleanAll();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	size_t ResourceManager::cleanAll()
	{
		return
			sounds.clean() +
			sprites.clean() +
			models.clean() +
			meshes.clean() +
			shaders.clean() +
			skyboxes.clean() +
			textures.clean() +
			images.clean() +
			fonts.clean();
	}

	size_t ResourceManager::reloadAll()
	{
		return
			fonts.reload() +
			images.reload() +
			meshes.reload() +
			models.reload() +
			shaders.reload() +
			skyboxes.reload() +
			sounds.reload() +
			sprites.reload() +
			textures.reload();
	}

	bool ResourceManager::loadManifest(const Manifest & value)
	{
		for (const Manifest::TypePair & p : value.getDataMap())
		{
			const String & path = value.getPath();
			const String & type = p.first;
			const Manifest::FileMap & files = p.second;

			if (!loadManifestData(type, path, files))
			{
				return Debug::logError("Failed Loading Manifest");
			}
		}
		return true;
	}

	int32_t ResourceManager::loadManifestData(const String & type, const String & path, const Manifest::FileMap & files)
	{
		if (type.empty())
		{
			return ML_WARNING;
		}
		else if (type == "fonts"	&& !loadData(fonts, path, files))	{ return ML_FAILURE; }
		else if (type == "images"	&& !loadData(images, path, files))	{ return ML_FAILURE; }
		else if (type == "meshes"	&& !loadData(meshes, path, files))	{ return ML_FAILURE; }
		else if (type == "models"	&& !loadData(models, path, files))	{ return ML_FAILURE; }
		else if (type == "shaders"	&& !loadData(shaders, path, files)) { return ML_FAILURE; }
		else if (type == "skyboxes" && !loadData(skyboxes, path, files)){ return ML_FAILURE; }
		else if (type == "sounds"	&& !loadData(sounds, path, files))	{ return ML_FAILURE; }
		else if (type == "sprites"	&& !loadData(sprites, path, files)) { return ML_FAILURE; }
		else if (type == "textures" && !loadData(textures, path, files)){ return ML_FAILURE; }
		
		return ML_SUCCESS;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}