#ifndef _DEMO_H_
#define _DEMO_H_

/* * * * * * * * * * * * * * * * * * * * */

#include <INIReader.h>
#include <MemeCore/Time.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/InputState.h>
#include <MemeCore/FileSystem.h>
#include <MemeGraphics/Text.h>
#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/Shapes.h>
#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/FrameBuffer.h>
#include <MemeGraphics/BufferLayout.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeGraphics/Mesh.h>
#include <MemeAudio/Sound.h>
#include <MemeNet/Client.h>
#include <MemeNet/Server.h>
#include <MemeScript/Interpreter.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace demo // Settings
{
	struct Settings final
	{
		// [Assets]
		std::string		assetPath;		// Where's all the data?
		
		// [Window]
		std::string		title;			// Window Title
		uint32_t		width;			// Window Width
		uint32_t		height;			// Window Height
		
		// [Graphics]
		float			fieldOfView;	// Field of View
		float			minClipPersp;	// Min Clipping Range Perspective
		float			maxClipPersp;	// Max Clipping Range Perspective
		float			minClipOrtho;	// Min Clipping Range Orthographic
		float			maxClipOrtho;	// Max Clipping Range Orthographic
		
		// [Script]
		std::string		script;			// Script to run on start
		bool			showToks;		// Show Script Tokens
		bool			showTree;		// Show Script Syntax Tree
		bool			showItoP;		// Show Script Infix to Postfix

		// [Network]
		bool			isServer;		// Is Server?

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool loadFromFile(const std::string & filename)
		{
			INIReader ini(filename.c_str());
			if (ini.ParseError() == 0)
			{
				// [Assets]
				assetPath	= ini.Get("Assets", "sAssetPath", "../../../assets");
				
				// [Window]
				title		= ini.Get("Window", "sTitle", "Title");
				width		= ini.GetInteger("Window", "iWidth", 640);
				height		= ini.GetInteger("Window", "iHeight", 480);
				
				// [Graphics]
				fieldOfView = (float)ini.GetReal("Graphics", "dFieldOfView",  90.0);
				minClipPersp= (float)ini.GetReal("Graphics", "dMinClipPersp", 0.1);
				maxClipPersp= (float)ini.GetReal("Graphics", "dMaxClipPersp", 1000.0);
				minClipOrtho= (float)ini.GetReal("Graphics", "dMinClipOrtho", -1.0);
				maxClipOrtho= (float)ini.GetReal("Graphics", "dMaxClipOrtho", +1.0);
				
				// [Script]
				script		= ini.Get("Script", "sScript", "/scripts/hello.script");
				showToks	= ini.GetBoolean("Script", "bShowToks", false);
				showTree	= ini.GetBoolean("Script", "bShowTree", false);
				showItoP	= ini.GetBoolean("Script", "bShowItoP", false);

				// [Network]
				isServer	= ini.GetBoolean("Network", "bIsServer", false);

				return ml::Debug::Success;
			}
			return ml::Debug::LogError("Failed Loading Settings \"{0}\"", filename);
		}

		inline const std::string pathTo(const std::string & filename) const
		{
			return assetPath + filename;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static Settings settings;
}

/* * * * * * * * * * * * * * * * * * * * */

namespace demo // Resource Registry
{
	enum : int32_t
	{
		// Fonts
		MIN_FONT = -1,
		FNT_clacon,
		FNT_consolas,
		FNT_lucida_console,
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
		MAX_MODEL,

		// Vertex Arrays
		MIN_VAO = -1,
		VAO_cube,
		VAO_quad,
		VAO_text,
		MAX_VAO,

		// Vertex Buffers
		MIN_VBO = -1,
		VBO_cube,
		VBO_quad,
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
		SND_test,
		MAX_SOUND,
	};

	/* * * * * * * * * * * * * * * * * * * * */

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
	ml::Client		client;
	ml::Server		server;
}

/* * * * * * * * * * * * * * * * * * * * */

namespace demo // Resource Loading
{
	inline static bool loadFonts()
	{
		// Load Fonts
		if (ml::Debug::Log("Loading Fonts..."))
		{
			if (!fonts[FNT_clacon].loadFromFile(settings.pathTo("/fonts/clacon.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font");
			}
			if (!fonts[FNT_consolas].loadFromFile(settings.pathTo("/fonts/consolas.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font");
			}
			if (!fonts[FNT_lucida_console].loadFromFile(settings.pathTo("/fonts/lucida_console.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font");
			}
			if (!fonts[FNT_minecraft].loadFromFile(settings.pathTo("/fonts/minecraft.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadImages()
	{
		// Load Images
		if (ml::Debug::Log("Loading Images..."))
		{
			if (!images[IMG_icon].loadFromFile(settings.pathTo("/images/dean.png")))
			{
				return ml::Debug::LogError("Failed Loading Icon");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadTextures()
	{
		// Load Textures
		if (ml::Debug::Log("Loading Textures..."))
		{
			if (!textures[TEX_dean].loadFromFile(settings.pathTo("/images/dean.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture");
			}
			if (!textures[TEX_sanic].loadFromFile(settings.pathTo("/images/sanic.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture");
			}
			if (!textures[TEX_stone_dm].loadFromFile(settings.pathTo("/textures/stone/stone_dm.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture");
			}
			if (!textures[TEX_stone_hm].loadFromFile(settings.pathTo("/textures/stone/stone_hm.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture");
			}
			if (!textures[TEX_stone_nm].loadFromFile(settings.pathTo("/textures/stone/stone_nm.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadShaders()
	{
		// Load Shaders
		if (ml::Debug::Log("Loading Shaders..."))
		{
			if (!shaders[GL_basic3D].loadFromFile(settings.pathTo("/shaders/basic3D.shader")))
			{
				return ml::Debug::LogError("Failed Loading Shader: {0}", "Basic3D");
			}
			if (!shaders[GL_text].loadFromFile(settings.pathTo("/shaders/text.shader")))
			{
				return ml::Debug::LogError("Failed Loading Shader: {0}", "Text");
			}
			if (!shaders[GL_geometry].loadFromFile(settings.pathTo("/shaders/geometry.shader")))
			{
				return ml::Debug::LogError("Failed Loading Shader: {0}", "Geometry");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadMeshes()
	{
		// Load Meshes
		if (ml::Debug::Log("Loading Meshes..."))
		{
			if (!mesh[MESH_sphere8x6].loadFromFile(settings.pathTo("/meshes/sphere8x6.mesh")))
			{
				return ml::Debug::LogError("Failed Loading Mesh: {0}", "sphere8x6");
			}
			if (!mesh[MESH_sphere32x24].loadFromFile(settings.pathTo("/meshes/sphere32x24.mesh")))
			{
				return ml::Debug::LogError("Failed Loading Mesh: {0}", "sphere32x24");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadBuffers()
	{
		// Load Buffers
		if (ml::Debug::Log("Loading Buffers..."))
		{
			static const ml::BufferLayout layout({
				{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
				{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
				{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
			});

			// Cube
			vao[VAO_cube]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_cube]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(ml::Shapes::Cube::Vertices.contiguous());
			ibo[IBO_cube]
				.create(ml::GL::StaticDraw, ml::GL::UnsignedInt)
				.bind()
				.bufferData(ml::Shapes::Cube::Indices);
			layout.bind();
			ibo[IBO_cube].unbind();
			vbo[VBO_cube].unbind();
			vao[VAO_cube].unbind();


			// Quad
			vao[VAO_quad]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_quad]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(ml::Shapes::Quad::Vertices.contiguous());
			ibo[IBO_quad]
				.create(ml::GL::StaticDraw, ml::GL::UnsignedInt)
				.bind()
				.bufferData(ml::Shapes::Quad::Indices);
			layout.bind();
			ibo[IBO_quad].unbind();
			vbo[VBO_quad].unbind();
			vao[VAO_quad].unbind();


			// Text
			vao[VAO_text]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_text]
				.create(ml::GL::DynamicDraw)
				.bind()
				.bufferData(NULL, (ml::Glyph::VertexCount * ml::Vertex::Size));
			layout.bind();
			vbo[VBO_text].unbind();
			vao[VAO_text].unbind();
		}
		return ml::Debug::Success;
	}

	inline static bool loadAudio()
	{
		// disabled
		if (0 && ml::Debug::Log("Loading Audio..."))
		{

		}
		return ml::Debug::Success;
	}

	inline static bool loadNetwork()
	{
		// disabled
		if (0 && ml::Debug::Log("Loading Network..."))
		{

		}
		return ml::Debug::Success;
	}
}

/* * * * * * * * * * * * * * * * * * * * */

namespace demo // Game Loop
{
	// Load
	struct LoadEvent final
	{

	};
	inline static bool onLoad(const LoadEvent & ev)
	{
		if (ml::Debug::Log("Loading Resources..."))
		{
			return
				loadFonts()		&&
				loadImages()	&&
				loadTextures()	&&
				loadShaders()	&&
				loadMeshes()	&&
				loadBuffers()	&&
				loadAudio()		&&
				loadNetwork()	&&
				ml::Debug::Log("OK.");
		}
		return ml::Debug::Success;
	}

	// Init
	struct InitEvent final
	{
		ml::RenderWindow &		window;
	};
	inline static bool onInit(const InitEvent & ev)
	{
		if (ml::Debug::Log("Initializing..."))
		{
			// Setup Window
			ev.window.setCursor(ml::Window::Cursor::Normal);
			ev.window.setPosition((ml::VideoMode::desktop().size - ev.window.size()) / 2);
			ev.window.setViewport(ml::vec2i::Zero, ev.window.size());
			
			if(ml::Image & icon = (images[IMG_icon]))
			{
				ev.window.setIcons({ icon.flipVertically() });
			}

			// Orthographic
			proj[P_ortho] = ml::Transform::Ortho(
				0.0f, (float)ev.window.width(),
				0.0f, (float)ev.window.height(),
				settings.minClipOrtho, 
				settings.maxClipOrtho);

			// Perspective
			proj[P_persp] = ml::Transform::Perspective(
				settings.fieldOfView, 
				ev.window.aspect(), 
				settings.minClipPersp, 
				settings.maxClipPersp);

			// Views
			ml::vec3f camPos = { 0.0f, 0.0f, 3.0f };
			view[V_camera] = ml::Transform::LookAt(camPos, camPos + ml::vec3f::Back, ml::vec3f::Up);


			// Cube
			model[M_cube]
				.translate({ +3.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One);

			// Quad
			model[M_quad]
				.translate({ -3.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One * 5.f);

			// Static Text
			text[TXT_static]
				.setFont(&fonts[FNT_minecraft])
				.setFontSize(72)
				.setScale(ml::vec2f::One)
				.setPosition({ 32, 128 })
				.setColor(ml::Color::White)
				.setText("there is no need\nto be upset");
		}
		return ml::Debug::Log("{0}", ML_Time.elapsed());
	}

	
	// Update
	struct UpdateEvent final
	{
		ml::RenderWindow &		window;
		const ml::Duration &	elapsed;
		const ml::InputState &	input;
	};
	inline static void onUpdate(const UpdateEvent & ev)
	{
		// Window Title
		ev.window.setTitle(ml::StringUtility::Format(
			"{0} | {1} | {2} ({3} fps)",
			settings.title,
			ML_Time.elapsed(),
			ev.elapsed.delta(),
			ml::Time::calculateFPS(ev.elapsed.delta())
		));

		// Handle Input
		if (ev.input.getKeyDown(ml::KeyCode::Escape))
		{
			ev.window.close();
		}
	}

	
	// Draw
	struct DrawEvent final
	{
		ml::RenderWindow &		window;
		const ml::Duration &	elapsed;
	};
	inline static void onDraw(const DrawEvent & ev)
	{
		ev.window.clear(ml::Color::Violet);
		{
			// Begin 3D
			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			// Cube
			if (ml::Shader & shader = shaders[GL_basic3D])
			{
				(shader)
					.setUniform(ml::Uniform::Model, model[M_cube]
						.translate(ml::vec3f::Zero)
						.rotate(+ev.elapsed.delta(), ml::vec3f::One)
						.scale(ml::vec3f::One))
					.setUniform(ml::Uniform::View, view[V_camera])
					.setUniform(ml::Uniform::Proj, proj[P_persp])
					.setUniform(ml::Uniform::Color, ml::Color::White)
					.setUniform(ml::Uniform::Texture, textures[TEX_stone_dm])
					.bind();

				vao[VAO_cube].bind();
				vbo[VBO_cube].bind();
				ibo[IBO_cube].bind();
				{
					ml::OpenGL::drawElements(
						vao[VAO_cube].mode(),
						ibo[IBO_cube].count(),
						ibo[IBO_cube].type(),
						NULL);
				}
				ibo[IBO_cube].unbind();
				vbo[VBO_cube].unbind();
				vao[VAO_cube].unbind();
			}

			// Begin 2D
			ml::OpenGL::disable(ml::GL::CullFace);
			ml::OpenGL::disable(ml::GL::DepthTest);

			// Quad
			if (ml::Shader & shader = shaders[GL_basic3D])
			{
				(shader)
					.setUniform(ml::Uniform::Model, model[M_quad]
						.translate(ml::vec3f::Zero)
						.rotate(-ev.elapsed.delta(), ml::vec3f::Forward)
						.scale(ml::vec3f::One))
					.setUniform(ml::Uniform::View, view[V_camera])
					.setUniform(ml::Uniform::Proj, proj[P_persp])
					.setUniform(ml::Uniform::Color, ml::Color::White)
					.setUniform(ml::Uniform::Texture, textures[TEX_sanic])
					.bind();

				vao[VAO_quad].bind();
				vbo[VBO_quad].bind();
				ibo[IBO_quad].bind();
				{
					ml::OpenGL::drawElements(
						vao[VAO_quad].mode(),
						ibo[IBO_quad].count(),
						ibo[IBO_quad].type(),
						NULL);
				}
				ibo[IBO_quad].unbind();
				vbo[VBO_quad].unbind();
				vao[VAO_quad].unbind();
			}

			// Text
			if (ml::Shader & shader = shaders[GL_text])
			{
				static ml::RenderBatch batch(
					&vao[VAO_text],
					&vbo[VBO_text],
					&proj[P_ortho],
					NULL,
					&shader);

				static const uint32_t  fontSize = 32;
				static const ml::vec2f offset = { 0.0f, -(float)fontSize };
				static const ml::vec2f origin = { (float)fontSize, (float)ev.window.height() };
				static const ml::vec4f colors[MAX_FONT] = {
					ml::Color::Red,
					ml::Color::Green,
					ml::Color::Blue,
					ml::Color::White,
				};

				// Dynamic Text
				for (uint32_t i = (MIN_FONT + 1); i < MAX_FONT; i++)
				{
					ev.window.draw(text[TXT_dynamic]
						.setFont(&fonts[i])
						.setFontSize(fontSize)
						.setScale(ml::vec2f::One)
						.setPosition(origin + (offset * (float)(i + 1)))
						.setColor(colors[i])
						.setText(fonts[i].to_string() + " | " + ev.window.title())
					, batch);
				}

				// Static Text
				ev.window.draw(text[TXT_static], batch);
			}
		}
		ev.window.swapBuffers().pollEvents();
	}

	
	// Exit
	struct ExitEvent final
	{
	};
	inline static bool onExit(const ExitEvent & ev)
	{
		return ml::Debug::Log("{0}", ML_Time.elapsed());
	}
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_DEMO_H_