/* * * * * * * * * * * * * * * * * * * * */

#include <dirent.h>
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

/* * * * * * * * * * * * * * * * * * * * */

inline static void loadCommands()
{
	ML_Interpreter.addCommand(ml::Command("help", [](ml::Args & args)
	{
		for (auto n : ML_Interpreter.getCmdNames())
			std::cout << n << std::endl;
		return ml::Var().boolValue(true);
	}));

	ML_Interpreter.addCommand(ml::Command("ml::ConsoleUtility::pause", [](ml::Args & args)
	{
		return ml::Var().intValue(ml::ConsoleUtility::pause());
	}));

	ML_Interpreter.addCommand(ml::Command("clear", [](ml::Args & args)
	{
#ifdef ML_SYSTEM_WINDOWS
		system("cls");
#else
		system("clear");
#endif
		return ml::Var().boolValue(true);
	}));

	ML_Interpreter.addCommand(ml::Command("cd", [](ml::Args & args)
	{
		return ml::Var().boolValue(ML_FileSystem.changeDir(args.pop_front().front()));
	}));

	ML_Interpreter.addCommand(ml::Command("exist", [](ml::Args & args)
	{
		return ml::Var().boolValue(ML_FileSystem.fileExists(args.pop_front().front()));
	}));

	ML_Interpreter.addCommand(ml::Command("dir", [](ml::Args & args)
	{
		std::string dName = args.pop_front().empty() ? "./" : args.str();

		if (DIR* dir = opendir(dName.c_str()))
		{
			dirent* e;
			while ((e = readdir(dir)))
			{
				switch (e->d_type)
				{
				case DT_REG:
					std::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "";
					break;
				case DT_DIR:
					std::cout << (ml::FG::Blue | ml::BG::Green) << e->d_name << "/";
					break;
				case DT_LNK:
					std::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "@";
					break;
				default:
					std::cout << (ml::FG::Green | ml::BG::Black) << e->d_name << "*";
					break;
				}
				std::cout << ml::FMT() << std::endl;
			}
			closedir(dir);
			return ml::Var().boolValue(true);
		}
		std::cout << "Dir \'" << dName << "\' does not exist." << std::endl;
		return ml::Var().boolValue(false);
	}));
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
	ml::Debug::LogInfo("Window Stub");
	
	// Window
	ml::RenderWindow window;
	ml::Debug::LogInfo("Creating Window...");
	if (!window.create(
		settings.title,
		ml::VideoMode(settings.width, settings.height, 32),
		ml::Window::Default,
		ml::ContextSettings(3, 3, 24, 8, ml::ContextSettings::Compat, false, false)))
	{
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	window.cursorMode(ml::Window::CursorMode::Normal);
	window.setViewport(ml::vec2i::Zero, window.size());
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
	ml::Debug::LogInfo("Script Stub");

	loadCommands();

	ML_Interpreter.parser()->
		showToks(settings.showToks).
		showTree(settings.showTree).
		showItoP(settings.showItoP);

	ML_Interpreter.execScript(settings.assetPath + settings.bootScript);

	return EXIT_SUCCESS;
}

inline static int astStub()
{
	ml::Debug::LogInfo("AST Stub");

	loadCommands();

	using namespace ml;
	AST_Block root({

		new AST_Func("GenVAO", { }),
		new AST_Block({
			new AST_Return(new AST_String("Here"))
		}),
		
		new AST_Assign(
			OperatorType::OP_SET,
			new AST_Name("a"),
			new AST_Oper(
				OperatorType::OP_ADD,
				new AST_Call(new AST_Name("GenVAO"), { }),
				new AST_Call(new AST_Name("GenVAO"), { }))),
		
		new AST_Print(new AST_Name("a")),
		
		new AST_Return(new AST_Bool(true))
		});
	std::cout << root << std::endl;
	root.run();
	return ml::ConsoleUtility::pause(EXIT_SUCCESS);
}

inline static int netStub()
{
	ml::Debug::LogInfo("Net Stub");

	ml::Client client;

	return ml::ConsoleUtility::pause(EXIT_SUCCESS);
}

inline static int audioStub()
{
	ml::Debug::LogInfo("Audio Stub");

	ml::Sound sound;

	return ml::ConsoleUtility::pause(EXIT_SUCCESS);
}

inline static int graphicsStub()
{
	ml::Debug::LogInfo("Graphics Stub");

	// Window
	ml::Debug::LogInfo("Creating Window...");
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
	window.setViewport(ml::vec2i::Zero, window.size());
	window.setCentered();

	// Load Fonts
	ml::Debug::LogInfo("Loading Fonts...");
	ml::Font font;
	if (!font.loadFromFile(settings.assetPath + "/fonts/Consolas.ttf"))
	{
		ml::Debug::LogError("Failed Loading Font");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Images
	ml::Debug::LogInfo("Loading Images...");
	ml::Image image;
	if (!image.loadFromFile(settings.assetPath + "/images/dean.png"))
	{
		ml::Debug::LogError("Failed Loading Image");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Textures
	ml::Debug::LogInfo("Loading Textures...");
	ml::Texture texture;
	if (!texture.loadFromImage(image))
	{
		ml::Debug::LogError("Failed Loading Texture");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	// Load Shaders
	ml::Debug::LogInfo("Loading Shaders...");
	ml::Shader shaderBasic;
	if (!shaderBasic.loadFromFile(
		settings.assetPath + "/shaders/vs/basic_vs.shader",
		settings.assetPath + "/shaders/fs/basic_fs.shader"))
	{
		ml::Debug::LogError("Failed Loading Shader: {0}", "Basic");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	ml::Shader shader2D;
	if (!shader2D.loadFromFile(
		settings.assetPath + "/shaders/vs/draw2D_vs.shader",
		settings.assetPath + "/shaders/fs/draw2D_fs.shader"))
	{
		ml::Debug::LogError("Failed Loading Shader: {0}", "2D");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	ml::Shader shader3D;
	if (!shader3D.loadFromFile(
		settings.assetPath + "/shaders/vs/draw3D_vs.shader",
		settings.assetPath + "/shaders/fs/draw3D_fs.shader"))
	{
		ml::Debug::LogError("Failed Loading Shader: {0}", "3D");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}

	ml::Shader shaderGS;
	if (!shaderGS.loadFromFile(
		settings.assetPath + "/shaders/vs/drawLine_vs.shader",
		settings.assetPath + "/shaders/gs/drawLine_gs.shader",
		settings.assetPath + "/shaders/fs/drawLine_fs.shader"))
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
	ml::Debug::LogInfo("Loading Sprites...");
	ml::Sprite sprite(&shader2D, &texture);
	if (!sprite)
	{
		ml::Debug::LogError("Failed Loading Sprite");
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}
	sprite.color(ml::Color::White);
	sprite.transform().position(ml::vec3f::Zero);

	// Load Text
	ml::Debug::LogInfo("Loading Text...");
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
	ml::Debug::LogInfo("Loading Geometry...");

	ml::VAO vao(1);
	ml::VBO vbo(ml::GL::StaticDraw, ml::Shapes::Quad::Mesh.flattened());
	ml::IBO ibo(ml::GL::StaticDraw, ml::Shapes::Quad::Mesh.indices());
	ml::BufferLayout layout({
		{ 0, 3, ml::GL::Float, false, ml::Vertex::Size, 0, sizeof(float) },
		{ 1, 4, ml::GL::Float, false, ml::Vertex::Size, 3, sizeof(float) },
		{ 2, 2, ml::GL::Float, false, ml::Vertex::Size, 7, sizeof(float) },
	});
	layout.use();


	// Matricies
	const ml::mat4f proj_ortho = ml::Transform::Ortho(ml::vec2f::Zero, window.size());
	const ml::mat4f proj_persp = ml::Transform::Persp(90.f, window.getAspect(), 0.1f, 1000.f);

	ml::Transform view(ml::vec3f::Zero, ml::vec3f::One, ml::quat());
	ml::Transform model(ml::vec3f::Forward, ml::vec3f::One, ml::quat());


	// Loop
	ml::InputState	input;
	ml::Timer		loopTimer;
	while (window.isOpen())
	{
		loopTimer.start();
		input.beginStep();
		{
			// Draw
			window.clear(ml::Color::Violet);
			{
				window.disableFlag(ml::GL::CullFace);
				window.disableFlag(ml::GL::DepthTest);
				{
					shaderBasic.use();
					shaderBasic.setUniform(ml::Uniform::Color,		ml::Color::White);
					shaderBasic.setUniform(ml::Uniform::Model,		model);
					shaderBasic.setUniform(ml::Uniform::View,		view);
					shaderBasic.setUniform(ml::Uniform::Proj,		proj_ortho);
					shaderBasic.setUniform(ml::Uniform::Texture,	&texture);

					window.drawElements(ibo, ml::GL::Triangles, ml::GL::UnsignedInt);
				}
				window.enableFlag(ml::GL::CullFace);
				window.enableFlag(ml::GL::DepthTest);
			}
			window.swapBuffers();
			window.pollEvents();

			// Handle Input
			if (input.getKeyDown(ml::KeyCode::Escape))
			{
				window.close();
			}
		}
		input.endStep();
		loopTimer.stop();

		ml::TimePoint now = ML_Time.elapsed();

		window.title(ml::StringUtility::Format(
			"{0} | {1}ms | {2}{3}:{4}{5}:{6}{7}", 
			settings.title, 
			loopTimer.elapsed().millis(),
			(now.minutes() % 60) / 10 % 10,
			(now.minutes() % 60) % 10,
			(now.seconds() % 60) / 10 % 10,
			(now.seconds() % 60) % 10,
			(now.millis()) / 10 % 10,
			(now.millis()) % 10));
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
	case 3: return astStub();
	case 4: return netStub();
	case 5: return audioStub();
	case 6: return graphicsStub();
	default:
		std::cerr << "Unknown program: " << settings.program << std::endl;
		return ml::ConsoleUtility::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * */