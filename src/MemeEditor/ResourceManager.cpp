#include <MemeEditor/ResourceManager.hpp>
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		clearFonts();
		clearImages();
		clearMeshes();
		clearShaders();
		clearTextures();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Font * ResourceManager::loadFont(const String & name, const String & filename)
	{
		if (!name.empty() && !filename.empty())
		{
			Font * temp = new Font();
			if (temp->loadFromFile(filename))
			{
				m_fonts[name] = temp;
				return getFont(name);
			}
			else
			{
				delete temp;
			}
		}
		return NULL;
	}

	size_t ResourceManager::loadFonts(const FileMap & files)
	{
		size_t count = 0;
		for (FileMap::const_iterator it = files.cbegin(); it != files.cend(); it++)
		{
			if (const Font * temp = loadFont(it->first, it->second))
			{
				Debug::log("Loaded Font | {0} | {1}", it->first, it->second);
				count++;
			}
			else
			{
				Debug::logError("Failed Loading Font | {0} | {1}", it->first, it->second);
			}
		}
		return count;
	}

	size_t ResourceManager::clearFonts()
	{
		size_t count = 0;
		for (auto it = m_fonts.begin(); it != m_fonts.end(); it++)
		{
			if (it->second)
			{
				delete it->second;
				it->second = NULL;
				count++;
			}
		}
		m_fonts.clear();
		return count;
	}

	Font * ResourceManager::getFont(const String & name)
	{
		FontMap::iterator it;
		if ((it = m_fonts.find(name)) != m_fonts.end())
		{
			return it->second;
		}
		return NULL;
	}

	const ResourceManager::FontMap & ResourceManager::getAllFonts() const
	{
		return m_fonts;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Image * ResourceManager::loadImage(const String & name, const String & filename)
	{
		if (!name.empty() && !filename.empty())
		{
			Image * temp = new Image();
			if (temp->loadFromFile(filename))
			{
				m_images[name] = temp;
				return getImage(name);
			}
			else
			{
				delete temp;
			}
		}
		return NULL;
	}

	size_t ResourceManager::loadImages(const FileMap & files)
	{
		size_t count = 0;
		for (FileMap::const_iterator it = files.cbegin(); it != files.cend(); it++)
		{
			if (const Image * temp = loadImage(it->first, it->second))
			{
				Debug::log("Loaded Image | {0} | {1}", it->first, it->second);
				count++;
			}
			else
			{
				Debug::logError("Failed Loading Image | {0} | {1}", it->first, it->second);
			}
		}
		return count;
	}

	size_t ResourceManager::clearImages()
	{
		size_t count = 0;
		for (auto it = m_images.begin(); it != m_images.end(); it++)
		{
			if (it->second)
			{
				delete it->second;
				it->second = NULL;
				count++;
			}
		}
		m_images.clear();
		return count;
	}

	Image * ResourceManager::getImage(const String & name)
	{
		ImageMap::iterator it;
		if ((it = m_images.find(name)) != m_images.end())
		{
			return it->second;
		}
		return NULL;
	}

	const ResourceManager::ImageMap & ResourceManager::getAllImages() const
	{
		return m_images;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Mesh * ResourceManager::loadMesh(const String & name, const String & filename)
	{
		if (!name.empty() && !filename.empty())
		{
			Mesh * temp = new Mesh();
			if (temp->loadFromFile(filename))
			{
				m_meshes[name] = temp;
				return getMesh(name);
			}
			else
			{
				delete temp;
			}
		}
		return NULL;
	}

	size_t ResourceManager::loadMeshes(const FileMap & files)
	{
		size_t count = 0;
		for (FileMap::const_iterator it = files.cbegin(); it != files.cend(); it++)
		{
			if (const Mesh * temp = loadMesh(it->first, it->second))
			{
				Debug::log("Loaded Mesh | {0} | {1}", it->first, it->second);
				count++;
			}
			else
			{
				Debug::logError("Failed Loading Mesh | {0} | {1}", it->first, it->second);
			}
		}
		return count;
	}

	size_t ResourceManager::clearMeshes()
	{
		size_t count = 0;
		for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		{
			if (it->second)
			{
				delete it->second;
				it->second = NULL;
				count++;
			}
		}
		m_meshes.clear();
		return count;
	}

	Mesh * ResourceManager::getMesh(const String & name)
	{
		MeshMap::iterator it;
		if ((it = m_meshes.find(name)) != m_meshes.end())
		{
			return it->second;
		}
		return NULL;
	}

	const ResourceManager::MeshMap & ResourceManager::getAllMeshes() const
	{
		return m_meshes;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Shader * ResourceManager::loadShader(const String & name, const String & filename)
	{
		if (!name.empty() && !filename.empty())
		{
			Shader * temp = new Shader();
			if (temp->loadFromFile(filename))
			{
				m_shaders[name] = temp;
				return getShader(name);
			}
			else
			{
				delete temp;
			}
		}
		return NULL;
	}

	size_t ResourceManager::loadShaders(const FileMap & files)
	{
		size_t count = 0;
		for (FileMap::const_iterator it = files.cbegin(); it != files.cend(); it++)
		{
			if (const Shader * temp = loadShader(it->first, it->second))
			{
				Debug::log("Loaded Shader | {0} | {1}", it->first, it->second);
				count++;
			}
			else
			{
				Debug::logError("Failed Loading Shader | {0} | {1}", it->first, it->second);
			}
		}
		return count;
	}

	size_t ResourceManager::clearShaders()
	{
		size_t count = 0;
		for (auto it = m_shaders.begin(); it != m_shaders.end(); it++)
		{
			if (it->second)
			{
				delete it->second;
				it->second = NULL;
				count++;
			}
		}
		m_shaders.clear();
		return count;
	}

	Shader * ResourceManager::getShader(const String & name)
	{
		ShaderMap::iterator it;
		if ((it = m_shaders.find(name)) != m_shaders.end())
		{
			return it->second;
		}
		return NULL;
	}

	const ResourceManager::ShaderMap & ResourceManager::getAllShaders() const
	{
		return m_shaders;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Texture * ResourceManager::loadTexture(const String & name, const String & filename)
	{
		if (!name.empty() && !filename.empty())
		{
			Texture * temp = new Texture();
			if (temp->loadFromFile(filename))
			{
				m_textures[name] = temp;
				return getTexture(name);
			}
			else
			{
				delete temp;
			}
		}
		return NULL;
	}

	size_t ResourceManager::loadTextures(const FileMap & files)
	{
		size_t count = 0;
		for (FileMap::const_iterator it = files.cbegin(); it != files.cend(); it++)
		{
			if (const Texture * temp = loadTexture(it->first, it->second))
			{
				Debug::log("Loaded Texture | {0} | {1}", it->first, it->second);
				count++;
			}
			else
			{
				Debug::logError("Failed Loading Texture | {0} | {1}", it->first, it->second);
			}
		}
		return count;
	}

	size_t ResourceManager::clearTextures()
	{
		size_t count = 0;
		for (auto it = m_textures.begin(); it != m_textures.end(); it++)
		{
			if (it->second)
			{
				delete it->second;
				it->second = NULL;
				count++;
			}
		}
		m_textures.clear();
		return count;
	}

	Texture * ResourceManager::getTexture(const String & name)
	{
		TextureMap::iterator it;
		if ((it = m_textures.find(name)) != m_textures.end())
		{
			return it->second;
		}
		return NULL;
	}

	const ResourceManager::TextureMap & ResourceManager::getAllTextures() const
	{
		return m_textures;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}