#include "Demo.hpp"
#include <MemeCore/EventSystem.hpp>
#include <MemeWindow/WindowEvents.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <../thirdparty/include/dirent.h>
#else
#include <dirent.h>
#endif // ML_SYSTEM_WINDOWS

#include <imgui/imgui.h>
#include <imgui/imgui_impl_ml.hpp>


namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	Demo::Demo()
		: m_status(ml::Debug::Success)
	{
		ML_EventSystem.addListener(DemoEvent::EV_Enter, this);
		ML_EventSystem.addListener(DemoEvent::EV_Load,	this);
		ML_EventSystem.addListener(DemoEvent::EV_Start, this);
		ML_EventSystem.addListener(DemoEvent::EV_Update,this);
		ML_EventSystem.addListener(DemoEvent::EV_Draw,	this);
		ML_EventSystem.addListener(DemoEvent::EV_Exit,	this);
	}

	Demo::~Demo()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Demo::onEvent(const ml::Event * value)
	{
		ml::RenderWindow::onEvent(value);

		switch (value->eventID())
		{
		case DemoEvent::EV_Enter:	onEnter(*value->Cast<EnterEvent>());	break;
		case DemoEvent::EV_Load:	onLoad(*value->Cast<LoadEvent>());		break;
		case DemoEvent::EV_Start:	onStart(*value->Cast<StartEvent>());	break;
		case DemoEvent::EV_Update:	onUpdate(*value->Cast<UpdateEvent>());	break;
		case DemoEvent::EV_Draw:	onDraw(*value->Cast<DrawEvent>());		break;
		case DemoEvent::EV_Exit:	onExit(*value->Cast<ExitEvent>());		break;

		case ml::WindowEvent::EV_WindowSize:
			if (auto ev = value->Cast<ml::WindowSizeEvent>())
			{
				// Orthographic
				proj[P_ortho] = ml::Transform::Orthographic(
					0.0f, (float)this->width(),
					0.0f, (float)this->height(),
					SETTINGS.orthoNear,
					SETTINGS.orthoFar);

				// Perspective
				proj[P_persp] = ml::Transform::Perspective(
					SETTINGS.fieldOfView,
					this->aspect(),
					SETTINGS.perspNear,
					SETTINGS.perspFar);
			}
			break;

		case ml::WindowEvent::EV_Char:
			if (auto ev = value->Cast<ml::CharEvent>())
			{
				if (m_acceptingInput)
				{
					text[TXT_input].setText(text[TXT_input].getText() + (char)ev->value);
				}
			}
			break;
		}

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

	bool Demo::loadFonts()
	{
		// Load Fonts
		return ml::Debug::log("Loading Fonts...")
			&& load<ml::Font>(fonts[FNT_clacon], "/fonts/clacon.ttf")
			&& load<ml::Font>(fonts[FNT_consolas], "/fonts/consolas.ttf")
			&& load<ml::Font>(fonts[FNT_lconsole], "/fonts/lucida_console.ttf")
			&& load<ml::Font>(fonts[FNT_minecraft], "/fonts/minecraft.ttf")
			;
	}

	bool Demo::loadImages()
	{
		// Load Images
		return ml::Debug::log("Loading Images...")
			&& load<ml::Image>(images[IMG_icon], "/images/dean.png")
			;
	}

	bool Demo::loadTextures()
	{
		// Load Textures
		return ml::Debug::log("Loading Textures...")
			&& load<ml::Texture>(textures[TEX_dean], "/images/dean.png")
			&& load<ml::Texture>(textures[TEX_sanic], "/images/sanic.png")
			&& load<ml::Texture>(textures[TEX_earth], "/images/earth.png")
			//&& load<ml::Texture>(textures[TEX_bg_clouds], "/textures/bg/bg_clouds.png")
			//&& load<ml::Texture>(textures[TEX_sky_clouds], "/textures/bg/sky_clouds.png")
			//&& load<ml::Texture>(textures[TEX_sky_water], "/textures/bg/sky_water.png")
			//&& load<ml::Texture>(textures[TEX_earth_cm], "/textures/earth/earth_cm_2k.png")
			//&& load<ml::Texture>(textures[TEX_earth_dm], "/textures/earth/earth_dm_2k.png")
			//&& load<ml::Texture>(textures[TEX_earth_hm], "/textures/earth/earth_hm_2k.png")
			//&& load<ml::Texture>(textures[TEX_earth_lm], "/textures/earth/earth_lm_2k.png")
			//&& load<ml::Texture>(textures[TEX_earth_nm], "/textures/earth/earth_nm_2k.png")
			//&& load<ml::Texture>(textures[TEX_earth_sm], "/textures/earth/earth_sm_2k.png")
			//&& load<ml::Texture>(textures[TEX_mars_dm], "/textures/mars/mars_dm_2k.png")
			//&& load<ml::Texture>(textures[TEX_mars_nm], "/textures/mars/mars_nm_2k.png")
			//&& load<ml::Texture>(textures[TEX_moon_dm], "/textures/moon/moon_dm_2k.png")
			//&& load<ml::Texture>(textures[TEX_moon_nm], "/textures/moon/moon_nm_2k.png")
			&& load<ml::Texture>(textures[TEX_stone_dm], "/textures/stone/stone_dm.png")
			&& load<ml::Texture>(textures[TEX_stone_hm], "/textures/stone/stone_hm.png")
			&& load<ml::Texture>(textures[TEX_stone_nm], "/textures/stone/stone_nm.png")
			;
	}

	bool Demo::loadShaders()
	{
		// Load Shaders
		return ml::Debug::log("Loading Shaders...")
			&& load<ml::Shader>(shaders[GL_basic3D], "/shaders/basic3D.shader")
			&& load<ml::Shader>(shaders[GL_sprites], "/shaders/sprites.shader")
			&& load<ml::Shader>(shaders[GL_text], "/shaders/text.shader")
			&& load<ml::Shader>(shaders[GL_geometry], "/shaders/geometry.shader")
			&& load<ml::Shader>(shaders[GL_framebuffer], "/shaders/framebuffer.shader")
			;
	}

	bool Demo::loadUniforms()
	{
		return ml::Debug::log("Loading Uniforms...")
			&& (uniforms[U_Model]	= "u_model")
			&& (uniforms[U_View]	= "u_view")
			&& (uniforms[U_Proj]	= "u_proj")
			&& (uniforms[U_Color]	= "u_color")
			&& (uniforms[U_Tex]		= "u_texture")
			&& (uniforms[U_Curve]	= "u_curveMode")
			;
	}

	bool Demo::loadMeshes()
	{
		// Load Meshes
		return ml::Debug::log("Loading Meshes...")
			&& load<ml::Mesh>(mesh[MESH_example], "/meshes/example.mesh")
			&& load<ml::Mesh>(mesh[MESH_sphere8x6], "/meshes/sphere8x6.mesh")
			&& load<ml::Mesh>(mesh[MESH_sphere32x24], "/meshes/sphere32x24.mesh")
			;
	}

	bool Demo::loadModels()
	{
		return ml::Debug::log("Loading Models...")
			;
	}

	bool Demo::loadBuffers()
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


			// Example
			vao[VAO_example]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_example]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(mesh[MESH_example].contiguous());
			layout.bind();
			vbo[VBO_example].unbind();
			vao[VAO_example].unbind();


			// Sphere 8x6
			vao[VAO_sphere8x6]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VBO_sphere8x6]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(mesh[MESH_sphere8x6].contiguous());
			layout.bind();
			vbo[VBO_sphere8x6].unbind();
			vao[VAO_sphere8x6].unbind();


			// Sphere 32x24
			vao[VAO_sphere32x24]
				.create(ml::GL::Triangles)
				.bind();
			vbo[VAO_sphere32x24]
				.create(ml::GL::StaticDraw)
				.bind()
				.bufferData(mesh[MESH_sphere32x24].contiguous());
			layout.bind();
			vbo[VBO_sphere32x24].unbind();
			vao[VAO_sphere32x24].unbind();

			
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


			// FBO
			fbo[FBO_scene]
				.create()
				.bind();
			// RBO
			rbo[RBO_scene]
				.create(this->width(), this->height())
				.bind()
				.bufferStorage(ml::GL::Depth24_Stencil8)
				.bufferFramebuffer(ml::GL::DepthStencilAttachment)
				.unbind();
			// Texture
			if (textures[TEX_framebuffer].create(
				rbo[RBO_scene].width(),
				rbo[RBO_scene].height()
			))
			{
				ml::OpenGL::framebufferTexture2D(
					ml::GL::Framebuffer, 
					ml::GL::ColorAttachment0,
					ml::GL::Texture2D,
					textures[TEX_framebuffer],
					0);
			}
			if (!ml::OpenGL::checkFramebufferStatus(ml::GL::Framebuffer))
			{
				return ml::Debug::logError("Framebuffer is not complete");
			}
			fbo[FBO_scene].unbind();
			
		}
		return true;
	}

	bool Demo::loadAudio()
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
			;
	}

	bool Demo::loadNetwork()
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

	void Demo::onEnter(const EnterEvent & ev)
	{
		// Start Master Timer
		ML_Time.start();

		// Setup Interpreter
		if (setupInterpreter())
		{
			// Run Script
			if (!SETTINGS.scrFile.empty())
			{
				m_status = ML_Interpreter.execFile(
					SETTINGS.pathTo(SETTINGS.scrPath + SETTINGS.scrFile)
				)
				? ml::Debug::Success
				: ml::Debug::Error;
			}
			else
			{
				m_status = ml::Debug::logWarning("No Script");
			}
		}
		else
		{
			m_status = ml::Debug::Error;
		}
	}
	
	void Demo::onLoad(const LoadEvent & ev)
	{
		if (ml::Debug::log("Creating Window...") && create(
			SETTINGS.title,
			ml::VideoMode({ SETTINGS.width, SETTINGS.height }, SETTINGS.bitsPerPixel),
			ml::Window::Style::Default,
			ml::Context(
				SETTINGS.majorVersion,
				SETTINGS.minorVersion,
				SETTINGS.profile,
				SETTINGS.depthBits,
				SETTINGS.stencilBits,
				SETTINGS.multisample,
				SETTINGS.srgbCapable)
		)
		&& setup())
		{
			this->setInputMode(ml::Cursor::Normal);
			this->setPosition((ml::VideoMode::desktop().size - this->size()) / 2);
			this->setViewport(ml::vec2i::Zero, this->size());

			m_status = (ml::Debug::log("Loading Resources..."))
				&& loadFonts()
				&& loadImages()
				&& loadTextures()
				&& loadShaders()
				&& loadUniforms()
				&& loadMeshes()
				&& loadModels()
				&& loadBuffers()
				&& loadAudio()
				&& loadNetwork()
				? ml::Debug::Success
				: ml::Debug::Error;
		}
		else
		{
			m_status = ml::Debug::logError("Failed Creating Window");
		}
	}
	
	void Demo::onStart(const StartEvent & ev)
	{
		if (ml::Debug::log("Dear ImGui..."))
		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			// Setup Platform/Renderer bindings
			ImGui_ML_InitForOpenGL(this, true);
			ImGui_ML_Init("#version 410");
		}


		if (ml::Debug::log("Starting..."))
		{
			// Set Window Icon
			if (ml::Image icon = images[IMG_icon])
			{
				this->setIcons({ icon.flipVertically() });
			}

			// Orthographic
			proj[P_ortho] = ml::Transform::Orthographic(
				0.0f, (float)this->width(),
				0.0f, (float)this->height(),
				SETTINGS.orthoNear,
				SETTINGS.orthoFar);

			// Perspective
			proj[P_persp] = ml::Transform::Perspective(
				SETTINGS.fieldOfView,
				this->aspect(),
				SETTINGS.perspNear,
				SETTINGS.perspFar);

			// Camera
			ml::vec3f cameraPos = { 0.0f, 0.0f, 3.0f };
			view[V_camera].lookAt(
				cameraPos,
				cameraPos + ml::vec3f::Back,
				ml::vec3f::Up);

			// Cube
			transform[T_cube]
				.translate({ +5.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One);

			// Quad
			transform[T_quad]
				.translate({ -5.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One);

			// Sphere 32x24
			transform[T_sphere32x24]
				.translate({ 0.0f, 0.0f, 0.0f })
				.rotate(0.0f, ml::vec3f::Up)
				.scale(ml::vec3f::One);

			// Static Text
			text[TXT_static]
				.setFont(&fonts[FNT_minecraft])
				.setFontSize(72)
				.setScale(ml::vec2f::One)
				.setPosition({ 32, 128 })
				.setColor(ml::Color::White)
				.setText("there is no need\nto be upset");

			// Input Text
			text[TXT_input]
				.setFont(&fonts[FNT_minecraft])
				.setFontSize(24)
				.setScale(ml::vec2f::One)
				.setPosition({ (float)this->width() / 2.f, 32 })
				.setColor(ml::Color::White)
				.setText(ml::String());
		}
	}
	
	void Demo::onUpdate(const UpdateEvent & ev)
	{
		// Update Title
		this->setTitle(ml::String::Format(
			"{0} | {1} | {2} ({3} fps) | {4}",
			SETTINGS.title,
			ML_Time.elapsed(),
			ev.elapsed.delta(),
			ml::Time::calculateFPS(ev.elapsed.delta()),
			this->getCursorPos()
		));


		// Handle Input
		if (ev.input.getKeyDown(ml::NativeKey::Escape))
		{
			this->close();
		}

		if (ev.input.getKeyDown(ml::NativeKey::Pause))
		{
			ml::Debug::pause(0);
		}

		if (ev.input.getKeyDown(ml::NativeKey::Tab))
		{
			m_acceptingInput = !m_acceptingInput;
		}

		if (m_acceptingInput)
		{
			if (ev.input.getKeyDown(ml::NativeKey::Return))
			{
				ml::String str;
				if (!(str = text[TXT_input].getText()).empty())
				{
					ML_Interpreter.execCommand(str);
				}
				text[TXT_input].setText(ml::String());
			}

			if (ev.input.getKeyDown(ml::NativeKey::Backspace))
			{
				ml::String str;
				if (!(str = text[TXT_input].getText()).empty())
				{
					str.pop_back();
					text[TXT_input].setText(str);
				}
			}
		}
		else
		{
			text[TXT_input].setText(ml::String());

			if (ev.input.getKeyDown(ml::NativeKey::R))
			{
				//loadShaders();
			}
			
			int32_t numPress = -1;
			for (int32_t i = ml::NativeKey::Num0; i <= ml::NativeKey::Num9; i++)
			{
				if (ev.input.getKeyDown(i))
				{
					numPress = (9 - (ml::NativeKey::Num9 - i));
					break;
				}
			}
			if (numPress >= 0)
			{
				m_fboMode = numPress;
				ml::Debug::log("FBO Mode: {0}", m_fboMode);
			}
		}



		// Update Network
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
		// Draw Scene
		/* * * * * * * * * * * * * * * * * * * * */
		fbo[FBO_scene].bind();
		{
			// Clear
			this->clear(ml::Color::Violet);
			
			// 3D
			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			// Cube
			if (const ml::Shader & s = shaders[GL_basic3D])
			{
				s.setUniform(uniforms[U_Color],	ml::Color::White);
				s.setUniform(uniforms[U_Tex],	textures[TEX_stone_dm]);
				s.setUniform(uniforms[U_Proj],	proj[P_persp]);
				s.setUniform(uniforms[U_View],	view[V_camera]);
				s.setUniform(uniforms[U_Model],	transform[T_cube]
					.translate(ml::vec3f::Zero)
					.rotate(+ev.elapsed.delta(), ml::vec3f::One)
					.scale(ml::vec3f::One));
				s.bind();

				this->draw(
					vao[VAO_cube],
					vbo[VBO_cube],
					ibo[IBO_cube]);
			}
			
			// Sphere32x24
			if (const ml::Shader & s = shaders[GL_basic3D])
			{
				s.setUniform(uniforms[U_Color],	ml::Color::White);
				s.setUniform(uniforms[U_Tex],	textures[TEX_earth]);
				s.setUniform(uniforms[U_Proj],	proj[P_persp]);
				s.setUniform(uniforms[U_View],	view[V_camera]);
				s.setUniform(uniforms[U_Model],	transform[T_sphere32x24]
					.translate(ml::vec3f::Zero)
					.rotate(+ev.elapsed.delta(), ml::vec3f::Up)
					.scale(ml::vec3f::One));
				s.bind();

				this->draw(
					vao[VAO_sphere32x24], 
					vbo[VBO_sphere32x24]);
			}

			// 2D
			ml::OpenGL::disable(ml::GL::CullFace);
			ml::OpenGL::disable(ml::GL::DepthTest);

			// Quad
			if (const ml::Shader & s = shaders[GL_sprites])
			{
				s.setUniform(uniforms[U_Color],	ml::Color::White);
				s.setUniform(uniforms[U_Tex],	textures[TEX_sanic]);
				s.setUniform(uniforms[U_Proj],	proj[P_persp]);
				s.setUniform(uniforms[U_View],	view[V_camera]);
				s.setUniform(uniforms[U_Model],	transform[T_quad]
					.translate(ml::vec3f::Zero)
					.rotate(-ev.elapsed.delta(), ml::vec3f::Forward)
					.scale(ml::vec3f::One));
				s.bind();

				this->draw(
					vao[VAO_quad], 
					vbo[VBO_quad], 
					ibo[IBO_quad]);
			}

			// Text
			if (true)
			{
				static ml::RenderBatch batch(
					&vao[VAO_text],
					&vbo[VBO_text],
					&proj[P_ortho],
					&shaders[GL_text]);

				static const uint32_t  fontSize = 24;
				static const ml::vec2f offset = { 0.0f, -(float)fontSize };
				static const ml::vec2f origin = { (float)fontSize, (float)this->height() };
				static const ml::vec4f colors[MAX_FONT] = {
					ml::Color::Red,
					ml::Color::Green,
					ml::Color::Blue,
					ml::Color::White,
				};

				// Dynamic Text
				for (uint32_t i = (MIN_FONT + 1); i < MAX_FONT; i++)
				{
					this->draw(text[TXT_dynamic]
						.setFont(&fonts[i])
						.setFontSize(fontSize)
						.setScale(ml::vec2f::One)
						.setPosition(origin + (offset * (float)(i + 1)))
						.setColor(colors[i])
						.setText(fonts[i].to_str())// + " | " + this->title())
					, batch);
				}

				// Static Text
				this->draw(text[TXT_static], batch);

				// Input Text
				this->draw(text[TXT_input], batch);
			}

			// Geometry
			if (const ml::Shader & s = shaders[GL_geometry])
			{
				s.setUniform(uniforms[U_Color], ml::Color::Red);
				s.setUniform(uniforms[U_Curve], 1);
				s.bind();
				ml::OpenGL::drawArrays(ml::GL::Points, 0, 4);
			}

		}
		fbo[FBO_scene].unbind();


		// Draw Framebuffer
		/* * * * * * * * * * * * * * * * * * * * */
		if(const ml::Shader & s = shaders[GL_framebuffer])
		{
			this->clear(ml::Color::White);

			s.setUniform(uniforms[U_Tex], textures[TEX_framebuffer]);
			s.setUniform("u_mode", m_fboMode);
			s.bind();
			
			this->draw(
				vao[VAO_quad],
				vbo[VBO_quad],
				ibo[IBO_quad]);
		}


		// Draw GUI
		/* * * * * * * * * * * * * * * * * * * * */
		this->pollEvents();
		{
			/* * * * * * * * * * * * * * * * * * * * */

			static bool show_demo_window = false;
			static bool show_another_window = false;

			/* * * * * * * * * * * * * * * * * * * * */

			ImGui_ML_NewFrame();
			ImGui::NewFrame();

			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)
			{
				ImGui::ShowDemoWindow(&show_demo_window);
			}

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");

				ImGui::Text("This is some useful text.");
				ImGui::Checkbox("Demo Window", &show_demo_window);
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				//ImGui::ColorEdit3("clear color", (float*)&clear_color);

				if (ImGui::Button("Button"))
				{
					counter++;
				}
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			// 3. Show another simple window.
			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
				{
					show_another_window = false;
				}
				ImGui::End();
			}

			// Rendering
			ImGui::Render();

			this->makeContextCurrent();
			
			this->setViewport(ml::vec2f::Zero, this->getFramebufferSize());
			
			ImGui_ML_RenderDrawData(ImGui::GetDrawData());
			
			this->makeContextCurrent();

			/* * * * * * * * * * * * * * * * * * * * */
		}
		this->swapBuffers();

		/* * * * * * * * * * * * * * * * * * * * */
	}
	
	void Demo::onExit(const ExitEvent & ev)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}