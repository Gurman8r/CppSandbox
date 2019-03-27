#ifndef _ML_RESOURCE_MANAGER_H_
#define _ML_RESOURCE_MANAGER_H_

#include <MemeEditor/ResourceTable.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/Plugin.hpp>
#include <MemeAudio/Sound.hpp>
#include <MemeGraphics/Effect.hpp>
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
		, public IReadable
		, public ISingleton<ResourceManager>
	{
		friend class ISingleton<ResourceManager>;

	public:
		using EffectMap		= ResourceTable<Effect>;
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

	private:
		ResourceManager();
		~ResourceManager();

	public:
		size_t cleanupAll();
		size_t reloadAll();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	private:
		bool parseFile(SStream & file);
		bool parseItem(const HashMap<String, String> & data);

	public:
		EffectMap	effects;
		FontMap		fonts;
		ImageMap	images;
		MaterialMap mats;
		MeshMap		meshes;
		ModelMap	models;
		PluginMap	plugins;
		ScriptMap	scripts;
		ShaderMap	shaders;
		SkyboxMap	skyboxes;
		SoundMap	sounds;
		SpriteMap	sprites;
		TextureMap	textures;

	private:
		std::vector<HashMap<String, String>> m_manifest;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_RESOURCE_MANAGER_H_