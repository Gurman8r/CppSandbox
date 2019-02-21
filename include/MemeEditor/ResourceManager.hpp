#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <MemeEditor/ResourceMap.hpp>
#include <MemeEditor/Manifest.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeCore/List.hpp>
#include <MemeAudio/Sound.hpp>
#include <MemeGraphics/Font.hpp>
#include <MemeGraphics/Model.hpp>
#include <MemeGraphics/Skybox.hpp>
#include <MemeGraphics/Sprite.hpp>
#include <MemeScript/Script.hpp>

#define ML_Res ml::ResourceManager::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ResourceManager
		: public ITrackable
		, public ISingleton<ResourceManager>
	{
		friend class ISingleton<ResourceManager>;

	public:
		using FontMap		= ResourceMap<Font>;
		using ImageMap		= ResourceMap<Image>;
		using MaterialMap	= ResourceMap<Material>;
		using MeshMap		= ResourceMap<Mesh>;
		using ModelMap		= ResourceMap<Model>;
		using ScriptMap		= ResourceMap<Script>;
		using ShaderMap		= ResourceMap<Shader>;
		using SkyboxMap		= ResourceMap<Skybox>;
		using SoundMap		= ResourceMap<Sound>;
		using SpriteMap		= ResourceMap<Sprite>;
		using TextureMap	= ResourceMap<Texture>;

		using file_map		= typename Manifest::file_map;
		using type_pair		= typename Manifest::type_pair;

	private:
		ResourceManager();
		~ResourceManager();

	public:
		size_t cleanAll();
		size_t reloadAll();

		bool loadManifest(const Manifest & value);

		bool loadManifestData(const String & type, const String & path, const file_map & files);

		template <class T>
		inline size_t loadData(ResourceMap<T> & data, const String & path, const file_map & files)
		{
			return data.load(files, path);
		}

	public:
		FontMap		fonts;
		ImageMap	images;
		MaterialMap mats;
		MeshMap		meshes;
		ModelMap	models;
		ScriptMap	scripts;
		ShaderMap	shaders;
		SkyboxMap	skyboxes;
		SoundMap	sounds;
		SpriteMap	sprites;
		TextureMap	textures;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_RESOURCE_MANAGER_H_
