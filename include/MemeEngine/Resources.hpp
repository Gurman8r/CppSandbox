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
		Registry<Effect>	effects;
		Registry<Font>		fonts;
		Registry<Image>		images;
		Registry<Material>	mats;
		Registry<Mesh>		meshes;
		Registry<Model>		models;
		Registry<Plugin>	plugins;
		Registry<Script>	scripts;
		Registry<Shader>	shaders;
		Registry<Skybox>	skyboxes;
		Registry<Sound> 	sounds;
		Registry<Sprite>	sprites;
		Registry<Texture>	textures;

	private:
		std::vector<ManifestItem> m_manifest;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCES_HPP_