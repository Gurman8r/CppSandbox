/* * * * * * * * * * * * * * * * * * * * */


#include <INIReader.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/Time.h>
#include <MemeCore/MemoryTracker.h>
#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/InputState.h>
#include <MemeCore/EventSystem.h>
#include <MemeCore/FileSystem.h>
#include <MemeCore/Entity.h>
#include <MemeAudio/Sound.h>
#include <MemeGraphics/Sprite.h>
#include <MemeGraphics/Text.h>
#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/Shapes.h>
#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/BufferLayout.h>
#include <MemeGraphics/OpenGL.h>
//#include <MemeGraphics/GLM.h>
#include <MemeNet/Client.h>
#include <MemeScript/Interpreter.h>

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

struct Settings final
	: public ml::ITrackable
{
	int			program;
	std::string	assetPath;

	std::string title;
	uint32_t	width;
	uint32_t	height;

	std::string bootScript;
	bool		showToks;
	bool		showTree;
	bool		showItoP;

	inline bool load(const std::string & filename)
	{
		INIReader ini(filename.c_str());
		if (ini.ParseError() == 0)
		{
			program		= ini.GetInteger("General", "iProgram", 0);
			assetPath	= ini.Get("General", "sAssetPath", "../../../assets");

			title		= ini.Get("Window", "sTitle", "Title");
			width		= ini.GetInteger("Window", "iWidth", 640);
			height		= ini.GetInteger("Window", "iHeight", 480);

			bootScript	= ini.Get("Script", "sBootScript", "/scripts/boot.script");
			showToks	= ini.GetBoolean("Script", "bShowToks", false);
			showTree	= ini.GetBoolean("Script", "bShowTree", false);
			showItoP	= ini.GetBoolean("Script", "bShowItoP", false);

			return true;
		}
		return false;
	}
};

static Settings settings;

/* * * * * * * * * * * * * * * * * * * * */

class TestComponent final
	: public ml::Component
{
public:
	TestComponent() { std::cout << (*this) << " Created" << std::endl; }
	~TestComponent() { std::cout << (*this) << " Destroyed" << std::endl; }
};

/* * * * * * * * * * * * * * * * * * * * */

inline static void delay(uint64_t value)
{
	static ml::Timer t;
	t.reset();
	while (t.elapsed().millis() < value);
}

template <typename T>
inline static void printBits(T value)
{
	std::cout
		<< std::left 
		<< std::setw(8)
		<< ml::FG::Yellow << value;
	for (unsigned i = 0, imax = (sizeof(value) * 8); i < imax; i++)
	{
		bool b = bitRead(value, i);

		std::cout << (b ? ml::FG::Green : ml::FG::Red) << b << " ";
	}
	std::cout << ml::FMT() << std::endl;
}

inline static uint64_t getFPS(float elapsedMS)
{
	static uint64_t	frameCounter = 0;
	static uint64_t	fps = 0;
	static float	nextSecond = 0.0f;
	static float	prevSecond = 0.0f;
	frameCounter++;
	nextSecond += elapsedMS;
	if (nextSecond - prevSecond > 1.0f)
	{
		prevSecond = nextSecond;
		fps = frameCounter;
		frameCounter = 0;
	}
	return fps;
}

/* * * * * * * * * * * * * * * * * * * * */

inline static int coreStub()
{
	std::cout << "Core Stub:" << std::endl;

	// Colors
	std::cout << "Colors:" << std::endl;
	char c = 64;
	for (uint16_t i = 0; i < ml::FG::MAX_COLOR; i++)
	{
		for (uint16_t j = 0; j < ml::BG::MAX_COLOR; j++)
		{
			std::cout
				<< (ml::FG::Values[i] | ml::BG::Values[j])
				<< (c = ((c < 127) ? c + 1 : 64)) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << ml::FMT() << std::endl << std::endl;


	// Vectors
	ml::vec2f vec2A = { 1.2f, 3.4f };
	ml::vec3f vec3B = { 5.6f, 7.8f, 9.0f };
	ml::vec3f vec3A = vec2A;
	ml::vec2f vec2B = vec3B;
	ml::vec4f vec4A = vec2A;
	std::cout
		<< "Vectors:" << std::endl
		<< "V2A: { " << vec2A << " }" << std::endl
		<< "V3A: { " << vec3A << " }" << std::endl
		<< "V3B: { " << vec3B << " }" << std::endl
		<< "V2B: { " << vec2B << " }" << std::endl
		<< "V4A: { " << vec4A << " }" << std::endl
		<< std::endl
		<< std::endl;


	// Check Maths
	ml::vec2f vec(-1.f, 1.f);
	std::cout
		<< "Checking Maths:" << std::endl
		<< std::left
		<< std::setw(12) << "Value: " << vec << std::endl
		<< std::setw(12) << "SqrMag: " << vec.sqrMagnitude() << std::endl
		<< std::setw(12) << "Magnitude: " << vec.magnitude() << std::endl
		<< std::setw(12) << "Normal: " << vec.normal() << std::endl
		<< std::setw(12) << "Front: " << vec.front() << std::endl
		<< std::setw(12) << "Back: " << vec.back() << std::endl
		<< std::setw(12) << "Empty: " << (vec.empty() ? "true" : "false") << std::endl
		<< std::endl
		<< std::endl;


	// Matrix Iterators
	ml::mat3f m3 = {
		2.2f,   4.4f,   6.6f,
		8.8f,   10.1f,  12.12f,
		14.14f, 16.16f, 18.18f,
	};
	std::cout << "M3: " << std::endl << m3 << std::endl << std::endl;
	for (ml::mat3f::iterator it = m3.begin(); it != m3.end(); it++)
	{
		std::size_t i = (it - m3.begin());
		std::cout
			<< "{" << i << ": " << (*it) << "}"
			<< ((it != m3.end() - 1) ? " " : "\n");;
	}
	std::cout << std::endl << std::endl;

	ml::mat4f m4 = ml::mat4f::identity();
	std::cout << "M4: " << std::endl << m4 << std::endl << std::endl;
	for (ml::mat4f::const_iterator it = m4.cbegin(); it != m4.cend(); it++)
	{
		std::size_t i = (it - m4.cbegin());
		std::cout
			<< "{" << i << ": " << (*it) << "}"
			<< ((it != m4.cend() - 1) ? " " : "\n");
	}
	std::cout << std::endl << std::endl;


	// Quaternaions
	ml::quat q1(1, 2, 3, 4);
	ml::quat q2 = ml::vec4f::One;
	ml::quat q3 = q1 * q2;
	ml::quat q4 = q1 * 1.5f;
	ml::quat q5 = ml::quat::slerp(q1, q2, 0.5f);

	std::cout
		<< std::left
		<< std::setw(12) << "Quaternions:" << std::endl
		<< std::setw(12) << "Q1" << q1 << std::endl
		<< std::setw(12) << "Q2" << q2 << std::endl
		<< std::setw(12) << "Q3" << q3 << std::endl
		<< std::setw(12) << "Q4" << q4 << std::endl
		<< std::setw(12) << "Q5" << q5 << std::endl
		<< std::setw(12) << "Q1 Complex" << q1.complex() << std::endl
		<< std::setw(12) << "Q1 Real" << q1.real() << std::endl
		<< std::endl
		<< std::endl;


	// Entity / Component
	ml::Entity* ent = new ml::Entity();
	ent->addComponent<TestComponent>();
	if (auto tmp = ent->getComponent<TestComponent>())
	{
		std::cout << (*tmp) << " OK" << std::endl;
	}
	delete ent;
	std::cout << std::endl;

	return ml::ConsoleUtility::pause(EXIT_SUCCESS);
}

inline static int windowStub()
{
	ml::Debug::Log("Window Stub");
	
	// Window
	ml::RenderWindow window;
	ml::Debug::Log("Creating Window...");
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

	// Loop
	ml::InputState	input;
	ml::Timer		loopTimer;
	while (window.isOpen())
	{
		loopTimer.start();
		input.beginStep();
		{
			window.clear(ml::Color::Violet);
			{
				if (input.getKeyDown(ml::KeyCode::Escape))
				{
					window.close();
				}
			}
			window.swapBuffers();
			window.pollEvents();
		}
		input.endStep();
		loopTimer.stop();

		window.title(ml::StringUtility::Format("{0} @ {1}ms",
			settings.title,
			loopTimer.elapsed().millis()));
	}

	return EXIT_SUCCESS;
}

inline static int scriptStub()
{
	ml::Debug::Log("Script Stub");

	ml::Interpreter::LoadBuiltinCommands();

	ML_Interpreter.parser()->
		showToks(settings.showToks).
		showTree(settings.showTree).
		showItoP(settings.showItoP);

	ML_Interpreter.execScript(settings.assetPath + settings.bootScript);

	return EXIT_SUCCESS;
}

inline static int netStub()
{
	ml::Debug::Log("Net Stub");

	ml::Client client;

	return ml::ConsoleUtility::pause(EXIT_SUCCESS);
}

inline static int audioStub()
{
	ml::Debug::Log("Audio Stub");

	ml::Sound sound;

	return ml::ConsoleUtility::pause(EXIT_SUCCESS);
}

inline static int graphicsStub()
{
	ml::Debug::Log("{0}", settings.title);

	// Enable GL Error Pause
	ml::OpenGL::errorPause(true);

	// Window
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

	// Load Fonts
	ml::Debug::Log("Loading Fonts...");
	ml::Font font;
	if (!font.loadFromFile(settings.assetPath + "/fonts/Consolas.ttf"))
	{
		ml::Debug::LogError("Failed Loading Font");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Images
	ml::Debug::Log("Loading Images...");
	ml::Image image;
	if (!image.loadFromFile(settings.assetPath + "/images/dean.png"))
	{
		ml::Debug::LogError("Failed Loading Image");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Textures
	ml::Debug::Log("Loading Textures...");
	ml::Texture texture;
	if (!texture.loadFromImage(image))
	{
		ml::Debug::LogError("Failed Loading Texture");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Shaders
	ml::Debug::Log("Loading Shaders...");
	ml::Shader shaderBasic;
	if (!shaderBasic.loadFromFile(
		settings.assetPath + "/shaders/vs/basic_vs.shader",
		settings.assetPath + "/shaders/fs/basic_fs.shader"))
	{
		ml::Debug::LogError("Failed Loading Shader: {0}", "Basic");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	ml::Shader shaderGS;
	if (!shaderGS.loadFromFile(
		settings.assetPath + "/shaders/vs/geometry_vs.shader",
		settings.assetPath + "/shaders/gs/geometry_gs.shader",
		settings.assetPath + "/shaders/fs/geometry_fs.shader"))
	{
		ml::Debug::LogError("Failed Loading Shader: {0}", "GS");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	ml::Shader shaderText;
	if (!shaderText.loadFromFile(
		settings.assetPath + "/shaders/vs/text_vs.shader",
		settings.assetPath + "/shaders/fs/text_fs.shader"))
	{
		ml::Debug::LogError("Failed Loading Shader: {0}", "Text");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Sprites
	ml::Debug::Log("Loading Sprites...");
	ml::Sprite sprite(&shaderBasic, &texture);
	if (!sprite)
	{
		ml::Debug::LogError("Failed Loading Sprite");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}
	sprite.color(ml::Color::White);
	sprite.transform().position(ml::vec3f::Zero);

	// Load Text
	ml::Debug::Log("Loading Text...");
	ml::Text text(&shaderText, &font);
	if (!text)
	{
		ml::Debug::LogError("Failed Loading Text");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}
	text.color(ml::Color::White);
	text.fontSize(14);
	text.string("Hello, World!");
	text.transform().position(ml::vec3f::Zero);

	// Load Geometry
	ml::Debug::Log("Loading Geometry...");

	const ml::Mesh & mesh = ml::Shapes::Cube::Mesh;

	ml::VAO vao(1);
	ml::VBO vbo(ml::GL::StaticDraw, mesh.flattened());
	ml::IBO ibo(ml::GL::StaticDraw, mesh.indices());	
	ml::BufferLayout layout({
		{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
		{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
		{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
	});
	layout.use();
	vao.unbind();
	vbo.unbind();
	ibo.unbind();

	const ml::mat4f proj_ortho = ml::Transform::Ortho(
		0.0f, (float)window.size()[0], 
		0.0f, (float)window.size()[1], 
		-1.0f, 1.0f);

	const ml::mat4f proj_persp = ml::Transform::Persp(
		90.0f, window.getAspect(), 0.1f, 1000.0f);
	
	ml::vec3f camPos = { 0.0f, 0.0f, 3.0f };
	ml::vec3f camFwd = { 0.0f, 0.0f, -1.0f };
	ml::vec3f camUp  = { 0.0f, 1.0f, 0.0f };
	ml::mat4f view = ml::Transform::LookAt(camPos, camPos + camFwd, camUp);

	ml::Transform model;
	model.translate({ 0.0f, 0.0f, 0.0f });
	model.rotate(0.0f, { 0.0f, 1.0f, 0.0f });
	model.scale({ 1.0f, 1.0f, 1.0f });

	// Done
	ml::Debug::Log("OK");

	// Loop
	ml::InputState	input;
	ml::Timer		loopTimer;
	ml::Duration	elapsed;
	while (window.isOpen())
	{
		loopTimer.start();
		input.beginStep();
		{
			// Draw
			window.clear(ml::Color::Violet);
			{
				model.translate({ 0.0f, 0.0f, 0.0f });
				model.rotate(elapsed.delta(), { 1.0f, 1.0f, 1.0f });
				model.scale({ 1.0f, 1.0f, 1.0f });

				ml::Shader * shader = &shaderBasic;
				shader->use();
				shader->setUniform(ml::Uniform::Proj, proj_persp);
				shader->setUniform(ml::Uniform::View, view);
				shader->setUniform(ml::Uniform::Model, model);
				shader->setUniform(ml::Uniform::Color, ml::Color::White);
				shader->setUniform(ml::Uniform::Texture, &texture);

				vao.bind();
				vbo.bind();
				window.drawElements(ibo, ml::GL::Triangles, ml::GL::UnsignedInt);
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
				getFPS(elapsed.delta()),
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

	std::cout << "OK" << std::endl;

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{	
	if (!settings.load(CONFIG_INI))
	{
		std::cerr << "Unable to locate config: \'" << CONFIG_INI << "\'." << std::endl;
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	ML_Time.start();
	
	switch (settings.program)
	{
	case 0: return coreStub();
	case 1: return windowStub();
	case 2: return scriptStub();
	case 3: return netStub();
	case 4: return audioStub();
	case 5: return graphicsStub();
	default:
		std::cerr << "Unknown program: " << settings.program << std::endl;
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * */