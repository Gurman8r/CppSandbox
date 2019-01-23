/* * * * * * * * * * * * * * * * * * * * */

#include <INIReader.h>
#include <MemeCore/Time.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>
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
#include <MemeScript/Interpreter.h>

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

struct Settings final
{
	std::string		assetPath;	// Where's all the data?
	std::string		title;		// Window Title
	uint32_t		width;		// Window Width
	uint32_t		height;		// Window Height

	std::string		bootScript;	// Script to run on start
	bool			showToks;	// Show Script Tokens
	bool			showTree;	// Show Script Syntax Tree
	bool			showItoP;	// Show Script Infix to Postfix

	inline bool load(const std::string & filename)
	{
		INIReader ini(filename.c_str());
		if (ini.ParseError() == 0)
		{
			assetPath	= ini.Get("Assets", "sAssetPath", "../../../assets");
			title		= ini.Get("Window", "sTitle", "Title");
			width		= ini.GetInteger("Window", "iWidth", 640);
			height		= ini.GetInteger("Window", "iHeight", 480);

			bootScript	= ini.Get("Script", "sBootScript", "/scripts/boot.script");
			showToks	= ini.GetBoolean("Script", "bShowToks", false);
			showTree	= ini.GetBoolean("Script", "bShowTree", false);
			showItoP	= ini.GetBoolean("Script", "bShowItoP", false);

			return true;
		}
		return ml::Debug::LogError("Failed Loading Settings \"{0}\"", filename);
	}

	inline const std::string pathTo(const std::string & filename) const
	{
		return assetPath + filename;
	}
};

static Settings settings;

/* * * * * * * * * * * * * * * * * * * * */

enum : int32_t
{
	/* Fonts
	* * * * * * * * * * * * * * * * * * * * */
	MIN_FONT = -1,
	FNT_clacon,
	FNT_consolas,
	FNT_lucida_console,
	FNT_minecraft,
	MAX_FONT,

	/* Images
	* * * * * * * * * * * * * * * * * * * * */
	MIN_IMAGE = -1,
	IMG_icon,
	MAX_IMAGE,

	/* Textures
	* * * * * * * * * * * * * * * * * * * * */
	MIN_TEXTURE = -1,
	TEX_dean,
	TEX_sanic,
	TEX_stone_dm,
	TEX_stone_hm,
	TEX_stone_nm,
	MAX_TEXTURE,

	/* Shaders
	* * * * * * * * * * * * * * * * * * * * */
	MIN_SHADER = -1,
	GL_basic,
	GL_text,
	GL_geometry,
	MAX_SHADER,

	/* Projection Matrices
	* * * * * * * * * * * * * * * * * * * * */
	MIN_PROJ = -1,
	P_persp,
	P_ortho,
	MAX_PROJ,

	/* View Matrices
	* * * * * * * * * * * * * * * * * * * * */
	MIN_VIEW = -1,
	V_camera,
	MAX_VIEW,

	/* Model Matrices
	* * * * * * * * * * * * * * * * * * * * */
	MIN_MODEL = -1,
	M_cube,
	M_quad,
	MAX_MODEL,

	/* VAOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_VAO = -1,
	VAO_cube,
	VAO_quad,
	VAO_batch,
	MAX_VAO,

	/* VBOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_VBO = -1,
	VBO_cube,
	VBO_quad,
	VBO_batch,
	MAX_VBO,

	/* IBOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_IBO = -1,
	IBO_cube,
	IBO_quad,
	MAX_IBO,

	/* FBOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_FBO = -1,
	FBO_test,
	MAX_FBO,

	/* Meshes
	* * * * * * * * * * * * * * * * * * * * */
	MIN_MESH = -1,
	MESH_sphere8x6,
	MESH_sphere32x24,
	MAX_MESH,

	/* Sounds
	* * * * * * * * * * * * * * * * * * * * */
	MIN_SOUND = -1,
	SND_test,
	MAX_SOUND,

	/* Text
	* * * * * * * * * * * * * * * * * * * * */
	MIN_TEXT = -1,
	TXT_dynamic,
	TXT_static,
	MAX_TEXT,
};

/* * * * * * * * * * * * * * * * * * * * */

ml::Font		fonts	[MAX_FONT];
ml::Image		images	[MAX_IMAGE];
ml::Texture		textures[MAX_TEXTURE];
ml::Shader		shaders	[MAX_SHADER];
ml::mat4f		proj	[MAX_PROJ];
ml::mat4f		view	[MAX_VIEW];
ml::Transform	model	[MAX_MODEL];
ml::VAO			vao		[MAX_VAO];
ml::VBO			vbo		[MAX_VBO];
ml::IBO			ibo		[MAX_IBO];
ml::FBO			fbo		[MAX_FBO];
ml::Mesh		meshes	[MAX_MESH];
ml::Sound		sounds	[MAX_SOUND];
ml::Text		text	[MAX_TEXT];

/* * * * * * * * * * * * * * * * * * * * */

inline static bool loadAssets()
{
	// Load Fonts
	if(ml::Debug::Log("Loading Fonts..."))
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

	// Load Images
	if (ml::Debug::Log("Loading Images..."))
	{
		if (!images[IMG_icon].loadFromFile(settings.pathTo("/images/dean.png")))
		{
			return ml::Debug::LogError("Failed Loading Icon");
		}
	}

	// Load Textures
	if(ml::Debug::Log("Loading Textures..."))
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

	// Load Shaders
	if(ml::Debug::Log("Loading Shaders..."))
	{
		if (!shaders[GL_basic].loadFromFile(settings.pathTo("/shaders/basic.shader")))
		{
			return ml::Debug::LogError("Failed Loading Shader: {0}", "Basic");
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

	return true;
}

inline static bool loadGeometry()
{
	// Load Meshes
	if (ml::Debug::Log("Loading Meshes"))
	{
		if (!meshes[MESH_sphere8x6].loadFromFile(settings.pathTo("/meshes/sphere8x6.mesh")))
		{
			return ml::Debug::LogError("Failed Loading Mesh: {0}", "sphere8x6");
		}

		//if (!meshes[MESH_sphere32x24].loadFromFile(settings.pathTo("/meshes/sphere32x24.mesh")))
		//{
		//	return ml::Debug::LogError("Failed Loading Mesh: {0}", "sphere32x24");
		//}
	}

	// Load Geometry
	if (ml::Debug::Log("Loading Geometry..."))
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
		vao[VAO_batch]
			.create(ml::GL::Triangles)
			.bind();
		vbo[VBO_batch]
			.create(ml::GL::DynamicDraw)
			.bind()
			.bufferData(NULL, (6 * ml::Vertex::Size));
		layout.bind();
		vbo[VBO_batch].unbind();
		vao[VAO_batch].unbind();
	}
	return true;
}

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{
	// Start master timer
	ML_Time.start();

	ml::Debug::Log("{0}", ML_Time.elapsed());

	// Load Settings
	if (!settings.load(CONFIG_INI))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Run Boot Script
	ml::Interpreter::LoadBuiltinCommands();
	(*ML_Interpreter.parser())
		.showToks(settings.showToks)
		.showTree(settings.showTree)
		.showItoP(settings.showItoP);
	ML_Interpreter.execScript(settings.pathTo(settings.bootScript));

	// Enable GL Error Pause
	ml::OpenGL::errorPause(true);

	// Create Window
	ml::Debug::Log("Creating Window...");
	ml::RenderWindow window;
	if (!window.create(
		settings.title,
		ml::VideoMode(settings.width, settings.height, 32),
		ml::Window::Style::Default,
		ml::Context(3, 3, 24, 8, ml::Context::Compat, false, false)))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Init
	if (ml::Debug::Log("Initializing..."))
	{
		// Setup Window
		window.setCursor(ml::Window::CursorMode::Normal);
		window.setPosition((ml::VideoMode::getDesktopMode().size() - window.size()) / 2);
		window.setViewport(ml::vec2i::Zero, window.size());

		// Load Assets
		if (!loadAssets())
		{
			return ml::Debug::pause(EXIT_FAILURE);
		}

		// Set Window Icon
		if (const ml::Image icon = ml::Image(images[IMG_icon]).flipVertically())
		{
			window.setIcons({ icon });
		}

		// Load Geometry
		if (!loadGeometry())
		{
			return ml::Debug::pause(EXIT_FAILURE);
		}

		// Projections
		proj[P_ortho] = ml::Transform::Ortho(0.0f, (float)window.width(), 0.0f, (float)window.height(), -1.0f, 1.0f);
		proj[P_persp] = ml::Transform::Perspective(90.0f, window.aspect(), 0.1f, 1000.0f);

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

	ml::Debug::Log("{0}", ML_Time.elapsed());

	// Loop
	ml::InputState	input;
	ml::Timer		loopTimer;
	ml::Duration	elapsed;
	do
	{	// Begin Step
		loopTimer.start();
		input.beginStep();

		// Update
		{
			// Window Title
			window.setTitle(ml::StringUtility::Format(
				"{0} | {1} | {2} ({3} fps)",
				settings.title,
				ML_Time.elapsed(),
				elapsed.delta(),
				ml::Time::calculateFPS(elapsed.delta())
			));

			// Handle Input
			if (input.getKeyDown(ml::KeyCode::Escape))
			{
				window.close();
			}
		}

		// Draw
		window.clear(ml::Color::Violet);
		{
			ml::OpenGL::enable(ml::GL::CullFace);
			ml::OpenGL::enable(ml::GL::DepthTest);

			// Cube
			if (ml::Shader & shader = shaders[GL_basic])
			{
				model[M_cube]
					.translate(ml::vec3f::Zero)
					.rotate(elapsed.delta(), ml::vec3f::One)
					.scale(ml::vec3f::One);

				(shader)
					.setUniform(ml::Uniform::Proj, proj[P_persp])
					.setUniform(ml::Uniform::View, view[V_camera])
					.setUniform(ml::Uniform::Model, model[M_cube])
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

			ml::OpenGL::disable(ml::GL::CullFace);
			ml::OpenGL::disable(ml::GL::DepthTest);

			// Quad
			if (ml::Shader & shader = shaders[GL_basic])
			{
				model[M_quad]
					.translate(ml::vec3f::Zero)
					.rotate(-elapsed.delta(), ml::vec3f::Forward)
					.scale(ml::vec3f::One);

				(shader)
					.setUniform(ml::Uniform::Proj, proj[P_persp])
					.setUniform(ml::Uniform::View, view[V_camera])
					.setUniform(ml::Uniform::Model, model[M_quad])
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
			if(ml::Shader & shader = shaders[GL_text])
			{
				static ml::RenderBatch batch(
					&vao[VAO_batch],
					&vbo[VBO_batch],
					&proj[P_ortho],
					NULL,
					&shader);

				static const uint32_t  fontSize = 32;
				static const ml::vec2f offset = { 0.0f, -(float)fontSize };
				static const ml::vec2f origin = { (float)fontSize, (float)window.height() };
				static const ml::vec4f colors[MAX_FONT] = {
					ml::Color::Red,
					ml::Color::Green,
					ml::Color::Blue,
					ml::Color::White,
				};
				
				// Dynamic Text
				for (uint32_t i = (MIN_FONT + 1); i < MAX_FONT; i++)
				{
					window.draw(text[TXT_dynamic]
						.setFont(&fonts[i])
						.setFontSize(fontSize)
						.setScale(ml::vec2f::One)
						.setPosition(origin + (offset * (float)(i + 1)))
						.setColor(colors[i])
						.setText(fonts[i].str() + " | " + window.title())
					, batch);
				}

				// Static Text
				window.draw(text[TXT_static], batch);
			}
		}
		window.swapBuffers().pollEvents();

		// End Step
		input.endStep();
		elapsed = loopTimer.stop().elapsed();
	}
	while (window.isOpen());

	ml::Debug::Log("{0}", ML_Time.elapsed());

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */