#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <MemeEditor/ResourceMap.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeCore/List.hpp>
#include <MemeAudio/Sound.hpp>
#include <MemeGraphics/Font.hpp>
#include <MemeGraphics/Model.hpp>
#include <MemeGraphics/Sprite.hpp>

#define ML_Resources ml::ResourceManager::getInstance()

namespace ml
{
	class ML_EDITOR_API ResourceManager
		: public ITrackable
		, public ISingleton<ResourceManager>
	{
		friend class ISingleton<ResourceManager>;

	protected:
		ResourceManager();
		~ResourceManager();

	public:
		void cleanAll();

	public:
		ResourceMap<Font>		fonts;
		ResourceMap<Image>		images;
		ResourceMap<Mesh>		meshes;
		ResourceMap<Model>		models;
		ResourceMap<Shader>		shaders;
		ResourceMap<Sound>		sounds;
		ResourceMap<Sprite>		sprites;
		ResourceMap<Texture>	textures;
	};

	
}
#endif // !_RESOURCE_MANAGER_H_
