#include <MemeLib/Property.h>
#include <MemeLib/Timer.h>
#include <MemeLib/MemoryTracker.h>
#include <MemeLib/ConsoleUtility.h>
#include <MemeLib/Entity.h>
#include <MemeLib/Vector2.h>
#include <MemeLib/Vector3.h>

class TestComponent : public ml::Component
{
public:
	TestComponent() {}
	TestComponent(const TestComponent & copy) {}
	~TestComponent() {}
};

int main(int argc, char** argv)
{
	std::vector<int> tVec;
	std::vector<int>::const_iterator test = tVec.cbegin();

	ml::Timer timer;
	timer.start();

	// Vectors
	ml::vec2f v2a = { 1.2f, 3.4f };
	ml::vec3f v3a = v2a;
	ml::vec3f v3b = { 5.6f, 7.8f, 9.0f };
	ml::vec2f v2b = v3b;
	std::cout 
		<< "V2 A: { " << v2a << " }" << std::endl
		<< "V3 A: { " << v3a << " }" << std::endl
		<< std::endl
		<< "V3 B: { " << v3b << " }" << std::endl
		<< "V2 B: { " << v2b << " }" << std::endl
		<< std::endl;

	// Iterators
	ml::mat3f m3 = {
		2.2f,  4.4f,  6.6f,
		8.8f,  10.1f, 12.12f,
		14.14f, 16.16f, 18.18f,
	};
	std::cout << m3 << std::endl << std::endl;
	for (auto it = m3.begin(); it != m3.end(); it++)
	{
		std::size_t i = (it - m3.begin());
		if (i == m3.Size / 2)
			(*it) = 1.2345f;
		std::cout << "[" << (i) << "] " << (*it) << std::endl;
	}
	std::cout << std::endl << std::endl;

	auto m4 = ml::mat4f();
	std::cout << m4 << std::endl << std::endl;
	for (auto it = m4.cbegin(); it != m4.cend(); it++)
	{
		std::size_t i = (it - m4.cbegin());
		std::cout << "[" << (i) << "] " << (*it) << std::endl;
	}
	std::cout << std::endl << std::endl;
	

	// Entity / Component
	ml::Entity* ent = new ml::Entity();
	ent->addComponent<TestComponent>();
	delete ent;

	// Properties
	std::cout
		<< ml::Property("B", true)		<< std::endl
		<< ml::Property("C", 'c')		<< std::endl
		<< ml::Property("D", 1.23)		<< std::endl
		<< ml::Property("F", 4.56f)		<< std::endl
		<< ml::Property("C", 789)		<< std::endl
		<< ml::Property("S", "Hello!")	<< std::endl
		<< std::endl;

	timer.stop();
	std::cout 
		<< "Elapsed: " << timer.elapsed().millis() << " msec" << std::endl
		<< std::endl;
	
	ml::ConsoleUtility::pause();
	
	return EXIT_SUCCESS;
}