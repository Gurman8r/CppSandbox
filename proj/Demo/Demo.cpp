#include "Demo.hpp"
#include <MemeCore/EventSystem.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <../thirdparty/include/dirent.h>
#else
#include <dirent.h>
#endif // ML_SYSTEM_WINDOWS


namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	Demo::Demo()
	{
	}

	Demo::~Demo()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Demo::onEnter(const EnterEvent & ev)
	{
		// Start Master Timer
		ML_Time.start();

		// Setup Interpreter
		if (setupInterpreter())
		{
			// Run Script
			if (!SETTINGS.scrFile.empty())
			{
				return ML_Interpreter.execFile(
					SETTINGS.pathTo(SETTINGS.scrPath + SETTINGS.scrFile)
				);
			}
			return ml::Debug::logWarning("No Script");
		}
		return false;
	}
	
	bool Demo::onLoad(const LoadEvent & ev)
	{
		return (ml::Debug::log("Loading..."))
			&& loadFonts(ev.log)
			&& loadImages(ev.log)
			&& loadTextures(ev.log)
			&& loadShaders(ev.log)
			&& loadMeshes(ev.log)
			&& loadBuffers(ev.log)
			&& loadAudio(ev.log)
			&& loadNetwork(ev.log)
			;
	}
	
	void Demo::onStart(const StartEvent & ev)
	{
		if (ml::Debug::log("Starting..."))
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
	
	void Demo::onUpdate(const UpdateEvent & ev)
	{
		// Set Window Title
		ev.window.setTitle(ml::String::Format(
			"{0} | {1} | {2} ({3} fps) | {4}",
			SETTINGS.title,
			ML_Time.elapsed(),
			ev.elapsed.delta(),
			ml::Time::calculateFPS(ev.elapsed.delta()),
			ev.window.getCursorPos()
		));

		// value_type Input
		if (ev.input.getKeyDown(ml::NativeKey::Escape))
		{
			ev.window.close();
		}

		if (SETTINGS.isServer)
		{
			ML_Server.poll();
		}
		else if(SETTINGS.isClient)
		{
			ML_Client.poll();
		}
	}
	
	void Demo::onDraw(const DrawEvent & ev)
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
					.setUniform(ml::Uniform::Color, ml::Color::White)
					.setUniform(ml::Uniform::Texture, textures[TEX_stone_dm])
					.setUniform(ml::Uniform::Proj, proj[P_persp])
					.setUniform(ml::Uniform::View, view[V_camera])
					.setUniform(ml::Uniform::Model, model[M_cube]
						.translate(ml::vec3f::Zero)
						.rotate(+ev.elapsed.delta(), ml::vec3f::One)
						.scale(ml::vec3f::One))
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
					.setUniform(ml::Uniform::Color, ml::Color::White)
					.setUniform(ml::Uniform::Texture, textures[TEX_sanic])
					.setUniform(ml::Uniform::Proj, proj[P_persp])
					.setUniform(ml::Uniform::View, view[V_camera])
					.setUniform(ml::Uniform::Model, model[M_quad]
						.translate(ml::vec3f::Zero)
						.rotate(-ev.elapsed.delta(), ml::vec3f::Forward)
						.scale(ml::vec3f::One))
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

				static const uint32_t  fontSize = 24;
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
						.setText(fonts[i].to_str())// + " | " + ev.window.title())
					, batch);
				}

				// Static Text
				ev.window.draw(text[TXT_static], batch);
			}
		}
		ev.window.swapBuffers().pollEvents();
	}
	
	int32_t Demo::onExit(const ExitEvent & ev)
	{
		return ev.exitCode;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Demo::setupInterpreter()
	{
		static bool checked = false;
		if (!checked)
		{
			checked = true;

			// Setup Parser
			ML_Interpreter.parser()
				.showToks(SETTINGS.scrShowToks)
				.showTree(SETTINGS.scrShowTree)
				.showItoP(SETTINGS.scrShowItoP);

			// Load Commands

			ML_Interpreter.addCmd({ "help", [](ml::Args & args)
			{
				for (auto n : ML_Interpreter.cmdNames())
				{
					ml::cout << n << ml::endl;
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
				ml::String buf;
				if (ML_FileSystem.getFileContents(args.pop_front().front(), buf))
				{
					ml::cout << buf << ml::endl;

					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "read", [](ml::Args & args)
			{

				ml::String name = args.pop_front().front();

				if (ML_FileSystem.fileExists(name))
				{
					ml::String buf;
					if (ML_FileSystem.getFileContents(name, buf))
					{
						return ml::Var().stringValue(buf);
					}
				}
				else
				{
					ml::String::Stream ss;
					if (ML_FileSystem.getDirContents(name, ss))
					{
						return ml::Var().stringValue(ss.str());
					}
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "exists", [](ml::Args & args)
			{
				auto name = args.pop_front().front();
				if (name == "." || name == "..")
				{
					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(ML_FileSystem.fileExists(name));
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
				const ml::String dirName = args.pop_front().empty() ? "./" : args.str();
				if (DIR * dir = opendir(dirName.c_str()))
				{
					while (dirent * e = readdir(dir))
					{
						switch (e->d_type)
						{
						case DT_REG:
							ml::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "";
							break;
						case DT_DIR:
							ml::cout << (ml::FG::Blue | ml::BG::Green) << e->d_name << "/";
							break;
						case DT_LNK:
							ml::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "@";
							break;
						default:
							ml::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "*";
							break;
						}
						ml::cout << ml::FMT() << ml::endl;
					}
					closedir(dir);
					return ml::Var().boolValue(true);
				}
				return ml::Var().boolValue(false);
			} });

			ML_Interpreter.addCmd({ "target", [](ml::Args & args)
			{
				if (!args.pop_front().empty())
				{
					const ml::String & opt = args.front();
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
		return checked;
	}

	bool Demo::loadFonts(bool log)
	{
		// Load Fonts
		return ml::Debug::log("Loading Fonts...")
			&& load<ml::Font>(fonts[FNT_clacon], "/fonts/clacon.ttf", log)
			&& load<ml::Font>(fonts[FNT_consolas], "/fonts/consolas.ttf", log)
			&& load<ml::Font>(fonts[FNT_lconsole], "/fonts/lucida_console.ttf", log)
			&& load<ml::Font>(fonts[FNT_minecraft], "/fonts/minecraft.ttf", log)
			;
	}

	bool Demo::loadImages(bool log)
	{
		// Load Images
		return ml::Debug::log("Loading Images...")
			&& load<ml::Image>(images[IMG_icon], "/images/dean.png", log)
			;
	}

	bool Demo::loadTextures(bool log)
	{
		ml::Debug::log("Max Texture Size: {0}", ml::vec2u(ml::OpenGL::getMaxTextureSize()))
			;

		// Load Textures
		return ml::Debug::log("Loading Textures...")
			&& load<ml::Texture>(textures[TEX_dean], "/images/dean.png", log)
			&& load<ml::Texture>(textures[TEX_sanic], "/images/sanic.png", log)
			//&& load<ml::Texture>(textures[TEX_bg_clouds], "/textures/bg/bg_clouds.png", log)
			//&& load<ml::Texture>(textures[TEX_sky_clouds], "/textures/bg/sky_clouds.png", log)
			//&& load<ml::Texture>(textures[TEX_sky_water], "/textures/bg/sky_water.png", log)
			//&& load<ml::Texture>(textures[TEX_earth_cm], "/textures/earth/earth_cm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_earth_dm], "/textures/earth/earth_dm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_earth_hm], "/textures/earth/earth_hm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_earth_lm], "/textures/earth/earth_lm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_earth_nm], "/textures/earth/earth_nm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_earth_sm], "/textures/earth/earth_sm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_mars_dm], "/textures/mars/mars_dm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_mars_nm], "/textures/mars/mars_nm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_moon_dm], "/textures/moon/moon_dm_2k.png", log)
			//&& load<ml::Texture>(textures[TEX_moon_nm], "/textures/moon/moon_nm_2k.png", log)
			&& load<ml::Texture>(textures[TEX_stone_dm], "/textures/stone/stone_dm.png", log)
			&& load<ml::Texture>(textures[TEX_stone_hm], "/textures/stone/stone_hm.png", log)
			&& load<ml::Texture>(textures[TEX_stone_nm], "/textures/stone/stone_nm.png", log)
			;
	}

	bool Demo::loadShaders(bool log)
	{
		// Load Shaders
		return ml::Debug::log("Loading Shaders...")
			&& load<ml::Shader>(shaders[GL_basic3D], "/shaders/basic3D.shader", log)
			&& load<ml::Shader>(shaders[GL_text], "/shaders/text.shader", log)
			&& load<ml::Shader>(shaders[GL_geometry], "/shaders/geometry.shader", log)
			;
	}

	bool Demo::loadMeshes(bool log)
	{
		// Load Meshes
		return ml::Debug::log("Loading Meshes...")
			&& load<ml::Mesh>(mesh[MESH_sphere8x6], "/meshes/sphere8x6.mesh", log)
			&& load<ml::Mesh>(mesh[MESH_sphere32x24], "/meshes/sphere32x24.mesh", log)
			&& load<ml::Mesh>(mesh[MESH_example], "/meshes/example.mesh", log)
			;
	}

	bool Demo::loadBuffers(bool log)
	{
		// Load Buffers
		if (ml::Debug::log("Loading Buffers..."))
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
		return true;
	}

	bool Demo::loadAudio(bool log)
	{
		if (ml::Debug::log("Loading Audio..."))
		{
			if (ml::OpenAL::init())
			{
				return true;
			}
			else
			{
				return ml::Debug::logError("Failed Loading Audio");
			}
		}
		return ml::Debug::log("Loading Sounds...")
			//&& load<ml::Sound>(sounds[SND_test], "/sounds/example.wav", log)
			;
	}

	bool Demo::loadNetwork(bool log)
	{
		if ((SETTINGS.isServer || SETTINGS.isClient) && 
			ml::Debug::log("Loading Network..."))
		{
			if (SETTINGS.isServer)
			{
				// Server Setup
				if (ML_Server.setup())
				{
					if (ML_Server.start({ ML_LOCALHOST, ML_PORT }, ML_MAX_CLIENTS))
					{
						ml::Debug::log("Server Started: {0}", ML_Server.getMyAddress());
						return true;
					}
				}
			}
			else if(SETTINGS.isClient)
			{
				// Client Setup
				if (ML_Client.setup())
				{
					if (ML_Client.connect({ ML_LOCALHOST, ML_PORT }, ""))
					{
						return ml::Debug::log("Client Connected: ");
					}
				}
			}
		}
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}