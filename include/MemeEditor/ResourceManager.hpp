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
	/* * * * * * * * * * * * * * * * * * * * */

	using FontMap		= ResourceMap<Font>;
	using ImageMap		= ResourceMap<Image>;
	using MeshMap		= ResourceMap<Mesh>;
	using ModelMap		= ResourceMap<Model>;
	using ShaderMap		= ResourceMap<Shader>;
	using SoundMap		= ResourceMap<Sound>;
	using SpriteMap		= ResourceMap<Sprite>;
	using TextureMap	= ResourceMap<Texture>;

	/* * * * * * * * * * * * * * * * * * * * */

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
		FontMap		fonts;
		ImageMap	images;
		MeshMap		meshes;
		ModelMap	models;
		ShaderMap	shaders;
		SoundMap	sounds;
		SpriteMap	sprites;
		TextureMap	textures;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_RESOURCE_MANAGER_H_
