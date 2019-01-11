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
	std::string		assetPath;	// 
	std::string		title;		// 
	uint32_t		width;		// 
	uint32_t		height;		// 
	std::string		bootScript;	// 
	bool			showToks;	// 
	bool			showTree;	// 
	bool			showItoP;	// 

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

			return ml::Debug::Log("OK");
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
	TEX_test,
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
	M_transform,
	MAX_MODEL,

	/* VAOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_VAO = -1,
	VAO_test,
	MAX_VAO,

	/* VBOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_VBO = -1,
	VBO_test,
	MAX_VBO,

	/* IBOs
	* * * * * * * * * * * * * * * * * * * * */
	MIN_IBO = -1,
	IBO_test,
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
	ml::Debug::Log("Loading Assets...");

	// Load Fonts
	ml::Debug::Log("Loading Fonts...");

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

	// Load Textures
	ml::Debug::Log("Loading Textures...");

	if (!textures[TEX_dean].loadFromFile(settings.pathTo("/images/dean.png")))
	{
		return ml::Debug::LogError("Failed Loading Texture");
	}

	// Load Shaders
	ml::Debug::Log("Loading Shaders...");

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

	// Load Sprites
	ml::Debug::Log("Loading Sprites...");

	if (!(sprites[SPR_dean] = ml::Sprite(&shaders[GL_basic], &textures[TEX_dean])))
	{
		return ml::Debug::LogError("Failed Loading Sprite");
	}

	// Load Text
	ml::Debug::Log("Loading Text...");

	if (!(text[TXT_default] = ml::Text(&shaders[GL_text], &fonts[FNT_consolas])))
	{
		return ml::Debug::LogError("Failed Loading Text");
	}

	// Load Geometry
	ml::Debug::Log("Loading Geometry...");
	const ml::Mesh & mesh = ml::Shapes::Cube::Mesh;

	vao[VAO_test].create(1);
	vbo[VBO_test].create(ml::GL::StaticDraw, mesh.flattened());
	ibo[IBO_test].create(ml::GL::StaticDraw, mesh.indices());
	ml::BufferLayout::bind({
		{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
		{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
		{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
	});
	vao[VAO_test].unbind();
	vbo[VBO_test].unbind();
	ibo[IBO_test].unbind();

	return ml::Debug::Log("OK");
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
	proj[P_ortho] = ml::Transform::Ortho(0.0f, (float)window.size()[0], 0.0f, (float)window.size()[1], -1.0f, 1.0f);
	proj[P_persp] = ml::Transform::Persp(90.0f, window.getAspect(), 0.1f, 1000.0f);

	ml::vec3f camPos = { 0.0f, 0.0f, 3.0f };
	view[V_camera] = ml::Transform::LookAt(camPos, camPos + ml::vec3f::Back, ml::vec3f::Up);

	model[M_transform].translate({ 0.0f, 0.0f, 0.0f });
	model[M_transform].rotate(0.0f, { 0.0f, 1.0f, 0.0f });
	model[M_transform].scale({ 1.0f, 1.0f, 1.0f });

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
			model[M_transform].translate({ 0.0f, 0.0f, 0.0f });
			model[M_transform].rotate(elapsed.delta(), { 1.0f, 1.0f, 1.0f });
			model[M_transform].scale({ 1.0f, 1.0f, 1.0f });

			// Draw
			window.clear(ml::Color::Violet);
			{
				if(ml::Shader * shader = &shaders[GL_basic])
				{
					shader->use();
					shader->setUniform(ml::Uniform::Proj,		proj[P_persp]);
					shader->setUniform(ml::Uniform::View,		view[V_camera]);
					shader->setUniform(ml::Uniform::Model,		model[M_transform]);
					shader->setUniform(ml::Uniform::Texture,	textures[TEX_test]);
					shader->setUniform(ml::Uniform::Color,		ml::Color::White);
				}
				vao[VAO_test].bind();
				vbo[VBO_test].bind();
				ibo[IBO_test].bind();
				{
					window.drawElements(ibo[IBO_test], ml::GL::Triangles, ml::GL::UnsignedInt);
				}
				vao[VAO_test].unbind();
				vbo[VBO_test].unbind();
				ibo[IBO_test].unbind();
			}
			window.swapBuffers();
			window.pollEvents();

			// Handle Input
			if (input.getKeyDown(ml::KeyCode::Escape))
			{
				window.close();
			}

			// Window Title
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