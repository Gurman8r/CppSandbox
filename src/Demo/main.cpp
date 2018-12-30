/* * * * * * * * * * * * * * * * * * * * */

#include <MemeLib/Property.h>
#include <MemeLib/Timer.h>
#include <MemeLib/MemoryTracker.h>
#include <MemeLib/ConsoleUtility.h>
#include <MemeLib/Entity.h>
#include <MemeLib/Vector2.h>
#include <MemeLib/Vector3.h>
#include <MemeLib/InputState.h>
#include <MemeLib/Maths.h>
#include <iomanip>

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

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{
	// Start Timer
	ml::Timer timer;
	timer.start();

	float f = ml::Maths::sign(-1.f);

	// Colors
	char c = 64;
	for (uint16_t i = 0; i < ml::FG::MAX_COLOR; i++)
	{
		for (uint16_t j = 0; j < ml::BG::MAX_COLOR; j++)
		{
			std::cout 
				<< (ml::FG::Values[i] | ml::BG::Values[j])
				<< (c = c < 127 ? c + 1 : 64) << " ";
		}
		std::cout << std::endl;
	}
	std::cout 
		<< ml::Fmt() << std::endl 
		<< std::endl;


	// Vectors
	ml::vec2f vec2A = { 1.2f, 3.4f };
	ml::vec3f vec3A = vec2A;
	ml::vec3f vec3B = { 5.6f, 7.8f, 9.0f };
	ml::vec2f vec2B = vec3B;
	std::cout 
		<< "V2 A: { " << vec2A << " }" << std::endl
		<< "V3 A: { " << vec3A << " }" << std::endl
		<< std::endl
		<< "V3 B: { " << vec3B << " }" << std::endl
		<< "V2 B: { " << vec2B << " }" << std::endl
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
		std::cout << "[" << (it - m3.begin()) << "] " << (*it) << std::endl;
	}
	std::cout << std::endl << std::endl;


	ml::mat4f m4 = ml::mat4f();
	std::cout << "M4: " << std::endl << m4 << std::endl << std::endl;
	for (ml::mat4f::const_iterator it = m4.cbegin(); it != m4.cend(); it++)
	{
		std::cout << "[" << (it - m4.cbegin()) << "] " << (*it) << std::endl;
	}
	std::cout << std::endl << std::endl;
	

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


	// Show Time
	timer.stop();
	std::cout 
		<< "Elapsed: " << timer.elapsed().millis() << " msec" << std::endl
		<< std::endl;


	// Exit
	std::cout << "Press Any Key to Continue..." << std::endl;
	ml::InputState input;
	while (!input.fullStep().getAnyKey());
	std::cout << std::endl;

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */