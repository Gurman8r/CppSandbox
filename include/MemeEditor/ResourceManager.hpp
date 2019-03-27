#ifndef _ML_RESOURCE_MANAGER_H_
#define _ML_RESOURCE_MANAGER_H_

#include <MemeEditor/ResourceTable.hpp>
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
		, public IReadable
		, public ISingleton<ResourceManager>
	{
		friend class ISingleton<ResourceManager>;

	public:
		using StringTable = HashMap<String, String>;

		using FontMap		= ResourceTable<Font>;
		using ImageMap		= ResourceTable<Image>;
		using MaterialMap	= ResourceTable<Material>;
		using MeshMap		= ResourceTable<Mesh>;
		using ModelMap		= ResourceTable<Model>;
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
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		void serialize(std::ostream & out) const override;

	private:
		bool parseFile(SStream & file);
		bool parseItems(SStream & file, String & line);
		bool parseValue(const StringTable & data);

	public:
		size_t cleanAll();
		size_t reloadAll();

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

	private:
		mutable std::vector<StringTable> m_manifest;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_RESOURCE_MANAGER_H_