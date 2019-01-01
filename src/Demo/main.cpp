/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/StringUtility.h>
#include <MemeCore/Timer.h>
#include <MemeCore/MemoryTracker.h>
#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/Entity.h>
#include <MemeCore/Vector2.h>
#include <MemeCore/Vector3.h>
#include <MemeCore/Quaternion.h>
#include <MemeCore/InputState.h>
#include <MemeCore/INIReader.h>
#include <MemeCore/EventSystem.h>
#include <MemeWindow/Window.h>
#include <MemeGraphics/Shader.h>
#include <MemeScript/Script.h>
#include <MemeScript/Property.h>

/* * * * * * * * * * * * * * * * * * * * */

class TestComponent : public ml::Component
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
	system("pause");
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

ml::Window window;
ml::Shader shader;
ml::Script script;
INIReader ini;

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{
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
		<< std::endl
		<< "V3B: { " << vec3B << " }" << std::endl
		<< "V2B: { " << vec2B << " }" << std::endl
		<< std::endl
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
	ml::quat q2 = ml::vec4f();
	ml::quat q3 = q1 * q2;
	ml::quat q4 = q1 * 1.5f;
	ml::quat q5 = ml::quat::slerp(q1, q2, 0.5f);
	std::cout
		<< std::left
		<< std::setw(10) << "Quaternions:" << std::endl
		<< std::setw(10) << "Q1" << q1 << std::endl
		<< std::setw(10) << "Q2" << q2 << std::endl
		<< std::setw(10) << "Q3" << q3 << std::endl
		<< std::setw(10) << "Q4" << q4 << std::endl
		<< std::setw(10) << "Q5" << q5 << std::endl
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


	// Properties
	std::cout
		<< "Properties:" << std::endl
		<< ml::Property("B", true)		<< std::endl
		<< ml::Property("C", 'c')		<< std::endl
		<< ml::Property("D", 1.23)		<< std::endl
		<< ml::Property("F", 4.56f)		<< std::endl
		<< ml::Property("C", 789)		<< std::endl
		<< ml::Property("S", "Hello!")	<< std::endl
		<< std::endl;

	// Window
	std::cout << "Creating Window..." << std::endl;
	switch (window.create("Demo", 
		ml::VideoMode(1280, 720, 32),
		ml::Window::Default,
		ml::ContextSettings(3, 3, 24, 8, ml::ContextSettings::Core, false, false)))
	{
	case ml::Window::ER_Invalid_Mode:
		std::cerr << "Video Mode is Invalid" << std::endl;
		return pause(EXIT_FAILURE);
	
	case ml::Window::ER_GLFW_Init_Failure:
		std::cerr << "Failed to Initialize GLFW" << std::endl;
		return pause(EXIT_FAILURE);
	
	case ml::Window::ER_GLFW_Create_Failure:
		std::cerr << "Failed to Create GLFW Window" << std::endl;
		return pause(EXIT_FAILURE);
	
	case ml::Window::ER_GLEW_Init_Failure:
		std::cerr << "Failed to Initialize GLEW" << std::endl;
		return pause(EXIT_FAILURE);
	
	case ml::Window::ER_Invalid_Handle:
		std::cerr << "Window Handle is Null" << std::endl;
		return pause(EXIT_FAILURE);
	}
	std::cout << "OK." << std::endl;

	window.setCursorMode(ml::Window::CursorMode::Normal);
	window.setViewport(ml::vec2i::Zero, window.getMode().size());
	window.setCentered();

	ml::Timer	timer;
	uint64_t	deltaTime = 0;

	while (window.isOpen())
	{
		timer.start();
		{
			window.clear({ 0.5f, 0.0f, 1.0f, 1.0f });
			{
				window.setTitle(ml::StringUtility::Format("Demo @ {0}ms", deltaTime));
			}
			window.swapBuffers(); 
			window.pollEvents();
		}
		timer.stop();

		deltaTime = timer.elapsed().millis();
	}

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */