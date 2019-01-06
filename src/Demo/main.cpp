/* * * * * * * * * * * * * * * * * * * * */

#include <dirent.h>
#include <INIReader.h>
#include <MemeCore/StringUtility.h>
#include <MemeCore/Timer.h>
#include <MemeCore/MemoryTracker.h>
#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/Entity.h>
#include <MemeCore/Vector2.h>
#include <MemeCore/Vector3.h>
#include <MemeCore/Quaternion.h>
#include <MemeCore/InputState.h>
#include <MemeCore/EventSystem.h>
#include <MemeCore/FileSystem.h>
#include <MemeWindow/Window.h>
#include <MemeGraphics/Shader.h>
#include <MemeGraphics/Color.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeGraphics/RenderWindow.h>
#include <MemeGraphics/Font.h>
#include <MemeScript/Interpreter.h>
#include <MemeNet/Client.h>
#include <MemeAudio/Sound.h>

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
			assetPath	= ini.Get("General", "sAssetPath", "../../../assets/");

			title		= ini.Get("Window", "sTitle", "Title");
			width		= ini.GetInteger("Window", "iWidth", 640);
			height		= ini.GetInteger("Window", "iHeight", 480);

			bootScript	= ini.Get("Script", "sBootScript", "boot.script");
			showToks	= ini.GetBoolean("Script", "bShowToks", false);
			showTree	= ini.GetBoolean("Script", "bShowTree", false);
			showItoP	= ini.GetBoolean("Script", "bShowItoP", false);

			return true;
		}
		return false;
	}
};

Settings settings;

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

inline static int32_t pause(int32_t exitCode = EXIT_SUCCESS)
{
#ifdef ML_SYSTEM_WINDOWS
	system("pause");
#else
	std::cout << "Press Any Key to Continue..." << std::endl;
	ml::InputState input;
	while (!input.beginStep().endStep().getAnyKey());
#endif
	return exitCode;
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

	ML_Interpreter.addCommand(ml::Command("pause", [](ml::Args & args)
	{
		return ml::Var().intValue(pause());
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

	return pause(EXIT_SUCCESS);
}

inline static int windowStub()
{
	std::cout << "Window Stub:" << std::endl;
	
	// Window
	ml::RenderWindow window;
	std::cout << "Creating Window..." << std::endl;
	switch (window.create(
		settings.title,
		ml::VideoMode(settings.width, settings.height, 32),
		ml::Window::Default,
		ml::ContextSettings(3, 3, 24, 8, ml::ContextSettings::Compat, false, false)))
	{
	case ml::Window::ER_Invalid_Mode:
		std::cerr << "Error: Video Mode is Invalid" << std::endl;
		return pause(EXIT_FAILURE);

	case ml::Window::ER_GLFW_Init_Failure:
		std::cerr << "Error: Failed to Initialize GLFW" << std::endl;
		return pause(EXIT_FAILURE);

	case ml::Window::ER_GLFW_Create_Failure:
		std::cerr << "Error: Failed to Create GLFW Window" << std::endl;
		return pause(EXIT_FAILURE);

	case ml::Window::ER_GLEW_Init_Failure:
		std::cerr << "Error: Failed to Initialize GLEW" << std::endl;
		return pause(EXIT_FAILURE);

	case ml::Window::ER_Invalid_Handle:
		std::cerr << "Error: Window Handle is Invalid" << std::endl;
		return pause(EXIT_FAILURE);

	case ml::Window::ER_SRGB_Failure:
		std::cerr << "Warning: Failed to enable SRGB" << std::endl;
		break;
	}
	std::cout << "OK." << std::endl;

	window.setCursorMode(ml::Window::CursorMode::Normal);
	window.setViewport(ml::vec2i::Zero, window.getSize());
	window.setCentered();

	// Loop
	ml::InputState	input;
	ml::Timer		loopTimer;
	uint64_t		elapsedMS = 0;
	while (window.isOpen())
	{
		loopTimer.start();
		input.beginStep();
		{
			window.setTitle(ml::StringUtility::Format("{0} @ {1}ms", settings.title, elapsedMS));

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
		elapsedMS = loopTimer.elapsed().millis();
	}

	return EXIT_SUCCESS;
}

inline static int scriptStub()
{
	std::cout << "Script Stub:" << std::endl;

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
	std::cout << "AST Stub:" << std::endl;

	loadCommands();

	using namespace ml;
	AST_Block root({

		new AST_Func("test", { }),
		new AST_Block({
			new AST_Return(new AST_String("Here"))
		}),
		
		new AST_Assign(
			OperatorType::OP_SET,
			new AST_Name("a"),
			new AST_Oper(
				OperatorType::OP_ADD,
				new AST_Call(new AST_Name("test"), { }),
				new AST_Call(new AST_Name("test"), { }))),
		
		new AST_Print(new AST_Name("a")),
		
		new AST_Return(new AST_Bool(true))
		});
	std::cout << root << std::endl;
	root.run();
	return pause(EXIT_SUCCESS);
}

inline static int netStub()
{
	std::cout << "Net Stub:" << std::endl;

	ml::Client client;

	return pause(EXIT_SUCCESS);
}

inline static int audioStub()
{
	std::cout << "Audio Stub:" << std::endl;

	ml::Sound sound;

	return pause(EXIT_SUCCESS);
}

inline static int graphicsStub()
{
	std::cout << "Graphics Stub:" << std::endl;

	ml::Font font;
	if (font.loadFromFile(settings.assetPath + "/fonts/Consolas.ttf"))
	{
		std::cout << "Loaded Font" << std::endl;
	}
	else
	{
		std::cerr << "Failed Loading Font" << std::endl;
		return pause(EXIT_FAILURE);
	}

	ml::Image image;
	if (image.loadFromFile(settings.assetPath + "/images/dean.png"))
	{
		std::cout << "Loaded Image" << std::endl;
	}
	else
	{
		std::cerr << "Failed Loading Image" << std::endl;
		return pause(EXIT_FAILURE);
	}

	ml::Shader shader;
	
	std::cout << "OK" << std::endl;
	return pause(EXIT_SUCCESS);
}

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{	
	if (!settings.load(CONFIG_INI))
	{
		std::cerr << "Unable to locate config: \'" << CONFIG_INI << "\'." << std::endl;
		return pause(EXIT_FAILURE);
	}
	
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
		return pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * */