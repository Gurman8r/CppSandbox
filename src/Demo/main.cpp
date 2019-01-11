/* * * * * * * * * * * * * * * * * * * * */

#include <INIReader.h>
#include <MemeCore/Time.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/InputState.h>
#include <MemeCore/FileSystem.h>
#include <MemeGraphics/Sprite.h>
#include <MemeGraphics/Text.h>
#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/Shapes.h>
#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/FrameBuffer.h>
#include <MemeGraphics/BufferLayout.h>
#include <MemeGraphics/OpenGL.h>
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
			assetPath	= ini.Get("Assets", "sPath", "../../../assets");
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

	/* Textures
	* * * * * * * * * * * * * * * * * * * * */
	MIN_TEXTURE = -1,
	TEX_dean,
	MAX_TEXTURE,

	/* Shaders
	* * * * * * * * * * * * * * * * * * * * */
	MIN_SHADER = -1,
	GL_basic,
	GL_text,
	GL_geometry,
	MAX_SHADER,

	/* Sprites
	* * * * * * * * * * * * * * * * * * * * */
	MIN_SPRITE = -1,
	SPR_dean,
	MAX_SPRITE,

	/* Text
	* * * * * * * * * * * * * * * * * * * * */
	MIN_TEXT = -1,
	TXT_default,
	MAX_TEXT,

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
	MAX_VAO,

	/* VBOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_VBO = -1,
	VBO_cube,
	VBO_quad,
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

	/* Sounds
	* * * * * * * * * * * * * * * * * * * * */
	MIN_SOUND = -1,
	SND_test,
	MAX_SOUND,
};

/* * * * * * * * * * * * * * * * * * * * */

ml::Font		fonts	[MAX_FONT];
ml::Texture		textures[MAX_TEXTURE];
ml::Shader		shaders	[MAX_SHADER];
ml::Sprite		sprites	[MAX_SPRITE];
ml::Text		text	[MAX_TEXT];
ml::mat4f		proj	[MAX_PROJ];
ml::mat4f		view	[MAX_VIEW];
ml::Transform	model	[MAX_MODEL];
ml::VAO			vao		[MAX_VAO];
ml::VBO			vbo		[MAX_VBO];
ml::IBO			ibo		[MAX_IBO];
ml::FBO			fbo		[MAX_FBO];
ml::Sound		sounds	[MAX_SOUND];

/* * * * * * * * * * * * * * * * * * * * */

inline static bool loadAssets()
{
	// Load Fonts
	ml::Debug::Log("Loading Fonts...");
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

	// Load Textures
	ml::Debug::Log("Loading Textures...");
	{
		if (!textures[TEX_dean].loadFromFile(settings.pathTo("/images/dean.png")))
		{
			return ml::Debug::LogError("Failed Loading Texture");
		}
	}

	// Load Shaders
	ml::Debug::Log("Loading Shaders...");
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

	// Load Sprites
	ml::Debug::Log("Loading Sprites...");
	{
		if (!(sprites[SPR_dean] = ml::Sprite(&shaders[GL_basic], &textures[TEX_dean])))
		{
			return ml::Debug::LogError("Failed Loading Sprite");
		}
	}

	// Load Text
	ml::Debug::Log("Loading Text...");
	{
		if (!(text[TXT_default] = ml::Text(&shaders[GL_text], &fonts[FNT_consolas])))
		{
			return ml::Debug::LogError("Failed Loading Text");
		}
	}

	// Load Geometry
	ml::Debug::Log("Loading Geometry...");
	{
		// Cube
		vao[VAO_cube].create(1);
		vbo[VBO_cube].create(ml::GL::StaticDraw, ml::Shapes::Cube::Mesh.flattened());
		ibo[IBO_cube].create(ml::GL::StaticDraw, ml::Shapes::Cube::Mesh.indices());
		ml::BufferLayout::bind({
			{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
			{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
			{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
		});
		vao[VAO_cube].unbind();
		vbo[VBO_cube].unbind();
		ibo[IBO_cube].unbind();

		// Quad
		vao[VAO_quad].create(1);
		vbo[VBO_quad].create(ml::GL::StaticDraw, ml::Shapes::Quad::Mesh.flattened());
		ibo[IBO_quad].create(ml::GL::StaticDraw, ml::Shapes::Quad::Mesh.indices());
		ml::BufferLayout::bind({
			{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
			{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
			{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
		});
		vao[VAO_quad].unbind();
		vbo[VBO_quad].unbind();
		ibo[IBO_quad].unbind();
	}

	return true;
}

/* * * * * * * * * * * * * * * * * * * * */

inline static uint64_t calcFPS(float deltaTime)
{
	static uint64_t	frameCounter = 0;
	static uint64_t	fps = 0;
	static float	nextSecond = 0.0f;
	static float	prevSecond = 0.0f;
	frameCounter++;
	nextSecond += deltaTime;
	if (nextSecond - prevSecond > 1.0f)
	{
		prevSecond = nextSecond;
		fps = frameCounter;
		frameCounter = 0;
	}
	return fps;
}

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{	
	// Start master timer
	ML_Time.start();

	// Load Settings
	if (!settings.load(CONFIG_INI))
	{
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Run Boot Script
	ml::Interpreter::LoadBuiltinCommands();
	ML_Interpreter.parser()->
		showToks(settings.showToks).
		showTree(settings.showTree).
		showItoP(settings.showItoP);
	ML_Interpreter.execScript(settings.pathTo(settings.bootScript));

	// Enable GL Error Pause
	ml::OpenGL::errorPause(true);

	// Create Window
	ml::Debug::Log("Creating Window...");
	ml::RenderWindow window;
	if (!window.create(
		settings.title,
		ml::VideoMode(settings.width, settings.height, 32),
		ml::Window::Default,
		ml::ContextSettings(3, 3, 24, 8, ml::ContextSettings::Compat, false, false)))
	{
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}
	window.cursorMode(ml::Window::CursorMode::Normal);
	window.viewport(ml::vec2i::Zero, window.size());
	window.setCentered();

	// Load Assets
	if (!loadAssets())
	{
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Matrices
	proj[P_ortho] = ml::Transform::Ortho(0.0f, (float)window.width(), 0.0f, (float)window.height(), -1.0f, 1.0f);
	proj[P_persp] = ml::Transform::Perspective(90.0f, window.aspect(), 0.1f, 1000.0f);

	ml::vec3f camPos = { 0.0f, 0.0f, 3.0f };
	view[V_camera] = ml::Transform::LookAt(camPos, camPos + ml::vec3f::Back, ml::vec3f::Up);

	model[M_cube]
		.translate({ 2.0f, 0.0f, 0.0f })
		.rotate(0.0f, ml::vec3f::Up)
		.scale(ml::vec3f::One);

	model[M_quad]
		.translate({ -2.0f, 0.0f, 0.0f })
		.rotate(0.0f, ml::vec3f::Up)
		.scale(ml::vec3f::One);

	// Loop
	ml::InputState	input;
	ml::Timer		loopTimer;
	ml::Duration	elapsed;
	while (window.isOpen())
	{
		loopTimer.start();
		input.beginStep();
		{
			// Update
			{
				// Cube
				model[M_cube]
					.translate(ml::vec3f::Zero)
					.rotate(elapsed.delta(), ml::vec3f::One)
					.scale(ml::vec3f::One);

				// Quad
				model[M_quad]
					.translate(ml::vec3f::Zero)
					.rotate(elapsed.delta(), ml::vec3f::Up)
					.scale(ml::vec3f::One);
			}

			// Draw
			window.clear(ml::Color::Violet);
			{
				// Cube
				if (ml::Shader * shader = &shaders[GL_basic])
				{
					shader->use()
						.setUniform(ml::Uniform::Proj,		proj[P_persp])
						.setUniform(ml::Uniform::View,		view[V_camera])
						.setUniform(ml::Uniform::Model,		model[M_cube])
						.setUniform(ml::Uniform::Color,		ml::Color::White)
						.setUniform(ml::Uniform::Texture,	textures[TEX_dean]);
				
					vao[VAO_cube].bind();
					vbo[VBO_cube].bind();
					ibo[IBO_cube].bind();
					{
						window.enable(ml::GL::CullFace);
						window.drawElements(ibo[IBO_cube], ml::GL::Triangles, ml::GL::UnsignedInt);
					}
					vao[VAO_cube].unbind();
					vbo[VBO_cube].unbind();
					ibo[IBO_cube].unbind();
				}

				// Quad
				if (ml::Shader * shader = &shaders[GL_basic])
				{
					shader->use()
						.setUniform(ml::Uniform::Proj,		proj[P_persp])
						.setUniform(ml::Uniform::View,		view[V_camera])
						.setUniform(ml::Uniform::Model,		model[M_quad])
						.setUniform(ml::Uniform::Color,		ml::Color::White)
						.setUniform(ml::Uniform::Texture,	textures[TEX_dean]);

					vao[VAO_quad].bind();
					vbo[VBO_quad].bind();
					ibo[IBO_quad].bind();
					{
						window.disable(ml::GL::CullFace);
						window.drawElements(ibo[IBO_quad], ml::GL::Triangles, ml::GL::UnsignedInt);
					}
					vao[VAO_quad].unbind();
					vbo[VBO_quad].unbind();
					ibo[IBO_quad].unbind();
				}
			}
			window.swapBuffers();
			window.pollEvents();

			// Handle Input
			if (input.getKeyDown(ml::KeyCode::Escape))
			{
				window.close();
			}

			// Set Window Title
			const ml::Duration & now = ML_Time.elapsed();
			window.title(ml::StringUtility::Format(
				"{0} | {1} ms | {2} fps | {3}",
				settings.title,
				elapsed.delta(),
				calcFPS(elapsed.delta()),
				(ml::StringUtility::Format("{0}{1}:{2}{3}:{4}{5}",
					(now.minutes() % 60) / 10 % 10,
					(now.minutes() % 60) % 10,
					(now.seconds() % 60) / 10 % 10,
					(now.seconds() % 60) % 10,
					(now.millis()) / 10 % 10,
					(now.millis()) % 10)))
			);
		}
		input.endStep();
		elapsed = loopTimer.stop().elapsed();
	}
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */