#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <MemeEditor/Loader.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeCore/List.hpp>
#include <MemeGraphics/Font.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/Mesh.hpp>


#define ML_Resources ml::ResourceManager::getInstance()

namespace ml
{
	class ML_EDITOR_API ResourceManager
		: public ITrackable
		, public ISingleton<ResourceManager>
	{
		friend class ISingleton<ResourceManager>;

	public:
		template <class T>
		using Map = std::unordered_map<String, T *>;

		using FileMap = std::unordered_map<String, String>;
		
		using FontMap		= Map<Font>;
		using ImageMap		= Map<Image>;
		using MeshMap		= Map<Mesh>;
		using ShaderMap		= Map<Shader>;
		using TextureMap	= Map<Texture>;

	public:
		ResourceManager();
		~ResourceManager();

	public: // Fonts
		const Font * loadFont(const String & name, const String & filename);
		size_t loadFonts(const FileMap & files);
		size_t clearFonts();
		Font * getFont(const String & name);
		const FontMap & getAllFonts() const;

	public: // Images
		const Image * loadImage(const String & name, const String & filename);
		size_t loadImages(const FileMap & files);
		size_t clearImages();
		Image * getImage(const String & name);
		const ImageMap & getAllImages() const;

	public: // Meshes
		const Mesh * loadMesh(const String & name, const String & filename);
		size_t loadMeshes(const FileMap & files);
		size_t clearMeshes();
		Mesh * getMesh(const String & name);
		const MeshMap & getAllMeshes() const;

	public: // Shaders
		const Shader * loadShader(const String & name, const String & filename);
		size_t loadShaders(const FileMap & files);
		size_t clearShaders();
		Shader * getShader(const String & name);
		const ShaderMap & getAllShaders() const;

	public: // Textures
		const Texture * loadTexture(const String & name, const String & filename);
		size_t loadTextures(const FileMap & files);
		size_t clearTextures();
		Texture * getTexture(const String & name);
		const TextureMap & getAllTextures() const;

	private:
		FontMap m_fonts;
		ImageMap m_images;
		MeshMap m_meshes;
		ShaderMap m_shaders;
		TextureMap m_textures;
	};

	
}
#endif // !_RESOURCE_MANAGER_H_
