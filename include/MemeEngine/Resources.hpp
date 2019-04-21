#ifndef _ML_RESOURCES_HPP_
#define _ML_RESOURCES_HPP_

#include <MemeAudio/Sound.hpp>
#include <MemeCore/List.hpp>
#include <MemeEngine/Registry.hpp>
#include <MemeEngine/ManifestItem.hpp>
#include <MemeEngine/Entity.hpp>
#include <MemeEngine/LuaScript.hpp>
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

	class ML_ENGINE_API Resources final
		: public ITrackable
		, public IDisposable
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
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	private:
		bool parseFile(SStream & file);
		bool parseItem(const ManifestItem & data);

	public:
		Registry<Effect>	effects;
		Registry<Entity>	entities;
		Registry<Font>		fonts;
		Registry<Image>		images;
		Registry<LuaScript>	lua;
		Registry<Mesh>		meshes;
		Registry<Model>		models;
		Registry<Plugin>	plugins;
		Registry<Script>	scripts;
		Registry<Shader>	shaders;
		Registry<Skybox>	skyboxes;
		Registry<Sound> 	sounds;
		Registry<Sprite>	sprites;
		Registry<Texture>	textures;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCES_HPP_