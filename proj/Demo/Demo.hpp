#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <MemeAudio/Audio.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/NativeInput.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/Shapes.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>
#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Model.hpp>
#include <MemeNet/Client.hpp>
#include <MemeNet/Server.hpp>
#include <MemeScript/Interpreter.hpp>

#include "DemoEvents.hpp"
#include "Settings.hpp"

// Resources
/* * * * * * * * * * * * * * * * * * * * */
namespace DEMO
{
	enum : int32_t
	{
		// Fonts
		MIN_FONT = -1,
		FNT_clacon,
		FNT_consolas,
		FNT_lconsole,
		FNT_minecraft,
		MAX_FONT,

		// Images
		MIN_IMAGE = -1,
		IMG_icon,
		MAX_IMAGE,

		// Textures
		MIN_TEXTURE = -1,
		TEX_dean,
		TEX_sanic,
		TEX_bg_clouds,
		TEX_sky_clouds,
		TEX_sky_water,
		TEX_earth_cm,
		TEX_earth_dm,
		TEX_earth_hm,
		TEX_earth_lm,
		TEX_earth_nm,
		TEX_earth_sm,
		TEX_mars_dm,
		TEX_mars_nm,
		TEX_moon_dm,
		TEX_moon_nm,
		TEX_stone_dm,
		TEX_stone_hm,
		TEX_stone_nm,
		MAX_TEXTURE,

		// Shaders
		MIN_SHADER = -1,
		GL_basic3D,
		GL_text,
		GL_geometry,
		MAX_SHADER,

		// Projection Matrices
		MIN_PROJ = -1,
		P_persp,
		P_ortho,
		MAX_PROJ,

		// View Matrices
		MIN_VIEW = -1,
		V_camera,
		MAX_VIEW,

		// Model Matrices
		MIN_MODEL = -1,
		M_cube,
		M_quad,
		M_sphere32x24,
		MAX_MODEL,

		// Vertex Arrays
		MIN_VAO = -1,
		VAO_cube,
		VAO_quad,
		VAO_example,
		VAO_sphere8x6,
		VAO_sphere32x24,
		VAO_text,
		MAX_VAO,

		// Vertex Buffers
		MIN_VBO = -1,
		VBO_cube,
		VBO_quad,
		VBO_example,
		VBO_sphere8x6,
		VBO_sphere32x24,
		VBO_text,
		MAX_VBO,

		// Index Buffers
		MIN_IBO = -1,
		IBO_cube,
		IBO_quad,
		MAX_IBO,

		// Frame Buffers
		MIN_FBO = -1,
		FBO_test,
		MAX_FBO,

		// Meshes
		MIN_MESH = -1,
		MESH_example,
		MESH_sphere8x6,
		MESH_sphere32x24,
		MAX_MESH,

		// Text
		MIN_TEXT = -1,
		TXT_dynamic,
		TXT_static,
		MAX_TEXT,

		// Sounds
		MIN_SOUND = -1,
		SND_sphere8x6,
		MAX_SOUND,
	};
}

// Demo
/* * * * * * * * * * * * * * * * * * * * */
namespace DEMO
{
	class Demo final
		: public ml::ITrackable
	{
	public:
		Demo();
		~Demo();

	public:
		bool	setupInterpreter();
		bool	loadFonts(bool log);
		bool	loadImages(bool log);
		bool	loadTextures(bool log);
		bool	loadShaders(bool log);
		bool	loadMeshes(bool log);
		bool	loadBuffers(bool log);
		bool	loadAudio(bool log);
		bool	loadNetwork(bool log);

		template <class T>
		inline static bool load(ml::IResource & res, const ml::String & file, bool log)
		{
			const std::type_info & info(typeid(res));
			return (res.loadFromFile(SETTINGS.pathTo(file))
				? (log
					? ml::Debug::log("Loaded \"{0}\"", file)
					: ml::Debug::Success)
				: ml::Debug::logError("Failed Loading [{0}]: \"{1}\"", info.name(), file)
				);
		}

	public:
		bool	onEnter(const EnterEvent & ev);
		bool	onLoad(const LoadEvent & ev);
		void	onStart(const StartEvent & ev);
		void	onUpdate(const UpdateEvent & ev);
		void	onDraw(const DrawEvent & ev);
		int32_t onExit(const ExitEvent & ev);

	private:
		ml::Font		fonts	[MAX_FONT];
		ml::Image		images	[MAX_IMAGE];
		ml::Texture		textures[MAX_TEXTURE];
		ml::Shader		shaders	[MAX_SHADER];
		ml::Transform	proj	[MAX_PROJ];
		ml::Transform	view	[MAX_VIEW];
		ml::Transform	model	[MAX_MODEL];
		ml::VAO			vao		[MAX_VAO];
		ml::VBO			vbo		[MAX_VBO];
		ml::IBO			ibo		[MAX_IBO];
		ml::FBO			fbo		[MAX_FBO];
		ml::Mesh		mesh	[MAX_MESH];
		ml::Text		text	[MAX_TEXT];
		ml::Sound		sounds	[MAX_SOUND];
	};
}

#endif // !_GAME_HPP_
