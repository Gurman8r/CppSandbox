#ifndef _DEMO_H_
#define _DEMO_H_

/* * * * * * * * * * * * * * * * * * * * */

#include "Settings.hpp"

#include <dirent.h>
#include <MemeAudio/Sound.h>
#include <MemeCore/Time.h>
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
#include <MemeNet/Client.h>
#include <MemeNet/Server.h>
#include <MemeScript/Interpreter.h>

// Resources
/* * * * * * * * * * * * * * * * * * * * */
namespace demo
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

// Loading
/* * * * * * * * * * * * * * * * * * * * */
namespace demo
{
	inline static void loadCommands()
	{
		static bool onlyOnce = true;
		if (onlyOnce)
		{	onlyOnce = false;

			ML_Interpreter.addCmd({ "help", [](ml::Args & args)
			{
				for (auto n : ML_Interpreter.cmdNames())
				{
					ml::Debug::out() << n << std::endl;
				}
				return ml::Var().boolValue(true);
			} });

			ML_Interpreter.addCmd({ "pause", [](ml::Args & args)
			{
				return ml::Var().intValue(ml::Debug::pause(EXIT_SUCCESS));
			} });

			ML_Interpreter.addCmd({ "clear", [](ml::Args & args)
			{
				return ml::Var().intValue(ml::Debug::clear());
			} });

			ML_Interpreter.addCmd({ "cwd", [](ml::Args & args)
			{
				return ml::Var().stringValue(ML_FileSystem.getWorkingDir());
			} });

			ML_Interpreter.addCmd({ "cd", [](ml::Args & args)
			{
				return ml::Var().boolValue(ML_FileSystem.setWorkingDir(args.pop_front().front()));
			} });

			ML_Interpreter.addCmd({ "cat", [](ml::Args & args)
			{
				std::string buf;
				if (ML_FileSystem.getFileContents(args.pop_front().front(), buf))
				{
					ml::Debug::out() << buf << std::endl;

					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "read", [](ml::Args & args)
			{
				std::string buf;
				if (ML_FileSystem.getFileContents(args.pop_front().front(), buf))
				{
					return ml::Var().stringValue(buf);
				}
				return ml::Var().stringValue(std::string());
			} });

			ML_Interpreter.addCmd({ "exists", [](ml::Args & args)
			{
				return ml::Var().boolValue(ML_FileSystem.fileExists(args.pop_front().front()));
			} });

			ML_Interpreter.addCmd({ "exec", [](ml::Args & args)
			{
				return ML_Interpreter.execFile(args.pop_front().front());
			} });

			ML_Interpreter.addCmd({ "system", [](ml::Args & args)
			{
				return ml::Var().intValue(system(args.pop_front().str().c_str()));
			} });

			ML_Interpreter.addCmd({ "ls", [](ml::Args & args)
			{
				const std::string dirName = args.pop_front().empty() ? "./" : args.str();
				if (DIR * dir = opendir(dirName.c_str()))
				{
					while (dirent * e = readdir(dir))
					{
						switch (e->d_type)
						{
						case DT_REG:
							ml::Debug::out() << (ml::FG::Green | ml::BG::Black) << e->d_name << "";
							break;
						case DT_DIR:
							ml::Debug::out() << (ml::FG::Blue | ml::BG::Green) << e->d_name << "/";
							break;
						case DT_LNK:
							ml::Debug::out() << (ml::FG::Green | ml::BG::Black) << e->d_name << "@";
							break;
						default:
							ml::Debug::out() << (ml::FG::Green | ml::BG::Black) << e->d_name << "*";
							break;
						}
						ml::Debug::out() << ml::FMT() << std::endl;
					}
					closedir(dir);
					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "target", [](ml::Args & args)
			{
				if(!args.pop_front().empty())
				{
					const std::string & opt = args.front();
					if (opt == "name")
					{
						return ml::Var().stringValue(SETTINGS.title);
					}
					else if (opt == "config")
					{
					#ifdef ML_DEBUG
						return ml::Var().stringValue("Debug");
					#else
						return ml::Var().stringValue("Release");
					#endif
					}
					else if (opt == "platform")
					{
					#if defined(ML_x64)
						return ml::Var().stringValue("x64");
					#elif defined(ML_x86)
						return ml::Var().stringValue("x86");
					#endif
					}
				}
				return ml::Var().boolValue(true);
			} });
		}
	};

	inline static bool loadFonts(bool en)
	{
		// Load Fonts
		if (en && ml::Debug::Log("Loading Fonts..."))
		{
			if (!fonts[FNT_clacon].loadFromFile(SETTINGS.pathTo("/fonts/clacon.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font {0}", "clacon.ttf");
			}

			if (!fonts[FNT_consolas].loadFromFile(SETTINGS.pathTo("/fonts/consolas.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font {0}", "consolas.ttf");
			}

			if (!fonts[FNT_lucida_console].loadFromFile(SETTINGS.pathTo("/fonts/lucida_console.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font {0}", "lucida_console.ttf");
			}

			if (!fonts[FNT_minecraft].loadFromFile(SETTINGS.pathTo("/fonts/minecraft.ttf")))
			{
				return ml::Debug::LogError("Failed Loading Font {0}", "minecraft.ttf");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadImages(bool en)
	{
		// Load Images
		if (en && ml::Debug::Log("Loading Images..."))
		{
			if (!images[IMG_icon].loadFromFile(SETTINGS.pathTo("/images/dean.png")))
			{
				return ml::Debug::LogError("Failed Loading Image {0}", "dean.png");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadTextures(bool en)
	{
		// Load Textures
		if (en && ml::Debug::Log("Loading Textures..."))
		{
			if (!textures[TEX_dean].loadFromFile(SETTINGS.pathTo("/images/dean.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture {0}", "dean.png");
			}

			if (!textures[TEX_sanic].loadFromFile(SETTINGS.pathTo("/images/sanic.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture {0}", "sanic.png");
			}

			if (!textures[TEX_stone_dm].loadFromFile(SETTINGS.pathTo("/textures/stone/stone_dm.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture {0}", "stone_dm.png");
			}

			if (!textures[TEX_stone_hm].loadFromFile(SETTINGS.pathTo("/textures/stone/stone_hm.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture {0}", "stone_hm.png");
			}

			if (!textures[TEX_stone_nm].loadFromFile(SETTINGS.pathTo("/textures/stone/stone_nm.png")))
			{
				return ml::Debug::LogError("Failed Loading Texture {0}", "stone_nm.png");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadShaders(bool en)
	{
		// Load Shaders
		if (en && ml::Debug::Log("Loading Shaders..."))
		{
			if (!shaders[GL_basic3D].loadFromFile(SETTINGS.pathTo("/shaders/basic3D.shader")))
			{
				return ml::Debug::LogError("Failed Loading Shader: {0}", "Basic3D");
			}

			if (!shaders[GL_text].loadFromFile(SETTINGS.pathTo("/shaders/text.shader")))
			{
				return ml::Debug::LogError("Failed Loading Shader: {0}", "Text");
			}

			if (!shaders[GL_geometry].loadFromFile(SETTINGS.pathTo("/shaders/geometry.shader")))
			{
				return ml::Debug::LogError("Failed Loading Shader: {0}", "Geometry");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadMeshes(bool en)
	{
		// Load Meshes
		if (en && ml::Debug::Log("Loading Meshes..."))
		{
			if (!mesh[MESH_sphere8x6].loadFromFile(SETTINGS.pathTo("/meshes/sphere8x6.mesh")))
			{
				return ml::Debug::LogError("Failed Loading Mesh: {0}", "sphere8x6");
			}

			if (!mesh[MESH_sphere32x24].loadFromFile(SETTINGS.pathTo("/meshes/sphere32x24.mesh")))
			{
				return ml::Debug::LogError("Failed Loading Mesh: {0}", "sphere32x24");
			}
		}
		return ml::Debug::Success;
	}

	inline static bool loadBuffers(bool en)
	{
		// Load Buffers
		if (en && ml::Debug::Log("Loading Buffers..."))
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

	inline static bool loadAudio(bool en)
	{
		if (en && ml::Debug::Log("Loading Audio..."))
		{
			// ...
		}
		return ml::Debug::Success;
	}

	inline static bool loadNetwork(bool en)
	{
		if (en && ml::Debug::Log("Loading Network..."))
		{
			if (SETTINGS.isServer)
			{
				// ...
			}
			else
			{
				// ...
			}
		}
		return ml::Debug::Success;
	}

}

// Events
/* * * * * * * * * * * * * * * * * * * * */
namespace demo
{
	// Program Enter
	/* * * * * * * * * * * * * * * * * * * * */
	struct ProgramEnterEvent final
	{
		int32_t argc;
		char ** argv;
	};
	// Called once at the top of main, after settings are loaded
	inline static bool onProgramEnter(const ProgramEnterEvent & ev)
	{
		// Start Master Timer
		ML_Time.start();

		// Load Commands
		loadCommands();
		
		// Setup Parser
		ML_Interpreter.parser()
			.showToks(SETTINGS.scrShowToks)
			.showTree(SETTINGS.scrShowTree)
			.showItoP(SETTINGS.scrShowItoP);

		// Run Script
		return ML_Interpreter.execFile(SETTINGS.pathTo(SETTINGS.scrFile));
	}


	// Load Resources
	/* * * * * * * * * * * * * * * * * * * * */
	struct LoadResourcesEvent final
	{
	};
	// Called once after the window is created
	inline static bool onLoadResources(const LoadResourcesEvent & ev)
	{
		return ml::Debug::Log("Loading...") 
			&& loadFonts	(1)
			&& loadImages	(1)
			&& loadTextures	(1)
			&& loadShaders	(1)
			&& loadMeshes	(1)
			&& loadBuffers	(1)
			&& loadAudio	(0)
			&& loadNetwork	(0)
			;
	}


	// Start
	/* * * * * * * * * * * * * * * * * * * * */
	struct StartEvent final
	{
		ml::RenderWindow & window;
	};
	// Called once before entering the main loop, after resources are loaded
	inline static void onStart(const StartEvent & ev)
	{
		if(ml::Debug::Log("Starting..."))
		{
			// Set Window Icon
			if (ml::Image icon = images[IMG_icon])
			{
				ev.window.setIcons({ icon.flipVertically() });
			}

			// Orthographic
			proj[P_ortho] = ml::Transform::Orthographic(
				0.0f, (float)ev.window.width(),
				0.0f, (float)ev.window.height(),
				SETTINGS.orthoNear,
				SETTINGS.orthoFar);

			// Perspective
			proj[P_persp] = ml::Transform::Perspective(
				SETTINGS.fieldOfView,
				ev.window.aspect(),
				SETTINGS.perspNear,
				SETTINGS.perspFar);

			// Camera
			ml::vec3f cameraPos = { 0.0f, 0.0f, 3.0f };
			view[V_camera].lookAt(
				cameraPos,
				cameraPos + ml::vec3f::Back,
				ml::vec3f::Up);

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
	}

	
	// Update
	/* * * * * * * * * * * * * * * * * * * * */
	struct UpdateEvent final
	{
		ml::RenderWindow & window;
		const ml::Duration & elapsed;
		const ml::InputState & input;
	};
	// Called once per frame, before draw
	inline static void onUpdate(const UpdateEvent & ev)
	{
		// Set Window Title
		ev.window.setTitle(ml::StringUtility::Format(
			"{0} | {1} | {2} ({3} fps)",
			SETTINGS.title,
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
	/* * * * * * * * * * * * * * * * * * * * */
	struct DrawEvent final
	{
		ml::RenderWindow & window;
		const ml::Duration & elapsed;
	};
	// Called once per frame, after update
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

	
	// Program Exit
	/* * * * * * * * * * * * * * * * * * * * */
	struct ProgramExitEvent final
	{
		int32_t exitCode;
	};
	// Called once at the end of main
	inline static int32_t onProgramExit(const ProgramExitEvent & ev)
	{
		return ev.exitCode;
	}
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_DEMO_H_