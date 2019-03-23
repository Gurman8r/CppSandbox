#ifndef _ML_RESOURCE_MANAGER_H_
#define _ML_RESOURCE_MANAGER_H_

#include <MemeEditor/ResourceTable.hpp>
#include <MemeEditor/ResourceManifest.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/Plugin.hpp>
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
		using FontMap		= ResourceTable<Font>;
		using ImageMap		= ResourceTable<Image>;
		using MaterialMap	= ResourceTable<Material>;
		using MeshMap		= ResourceTable<Mesh>;
		using ModelMap		= ResourceTable<Model>;
		using PluginMap		= ResourceTable<Plugin>;
		using ScriptMap		= ResourceTable<Script>;
		using ShaderMap		= ResourceTable<Shader>;
		using SkyboxMap		= ResourceTable<Skybox>;
		using SoundMap		= ResourceTable<Sound>;
		using SpriteMap		= ResourceTable<Sprite>;
		using TextureMap	= ResourceTable<Texture>;

		using file_map		= typename ResourceManifest::file_map;
		using type_pair		= typename ResourceManifest::type_pair;

	private:
		ResourceManager();
		~ResourceManager();

	public:
		size_t cleanAll();
		size_t reloadAll();

		bool loadManifest(const ResourceManifest & value);

		bool loadManifestData(const String & type, const String & path, const file_map & files);

		template <class T>
		inline size_t loadData(ResourceTable<T> & data, const String & path, const file_map & files)
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
#endif // !_ML_RESOURCE_MANAGER_H_