#ifndef _ML_RESOURCES_HPP_
#define _ML_RESOURCES_HPP_

#include <MemeEngine/Registry.hpp>
#include <MemeEngine/ManifestItem.hpp>
#include <MemeCore/List.hpp>
#include <MemeAudio/Sound.hpp>
#include <MemeEngine/Plugin.hpp>
#include <MemeGraphics/Effect.hpp>
#include <MemeGraphics/Font.hpp>
#include <MemeGraphics/Model.hpp>
#include <MemeGraphics/Skybox.hpp>
#include <MemeGraphics/Sprite.hpp>
#include <MemeScript/Script.hpp>

#define ML_Res ml::Resources::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Resources
		: public ITrackable
		, public IReadable
		, public ISingleton<Resources>
	{
		friend class ISingleton<Resources>;

	public:
		using EffectMap		= Registry<Effect>;
		using FontMap		= Registry<Font>;
		using ImageMap		= Registry<Image>;
		using MaterialMap	= Registry<Material>;
		using MeshMap		= Registry<Mesh>;
		using ModelMap		= Registry<Model>;
		using PluginMap		= Registry<Plugin>;
		using ScriptMap		= Registry<Script>;
		using ShaderMap		= Registry<Shader>;
		using SkyboxMap		= Registry<Skybox>;
		using SoundMap		= Registry<Sound>;
		using SpriteMap		= Registry<Sprite>;
		using TextureMap	= Registry<Texture>;

	private:
		Resources();
		~Resources();

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
		bool parseItem(const ManifestItem & data);

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
		std::vector<ManifestItem> m_manifest;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCES_HPP_