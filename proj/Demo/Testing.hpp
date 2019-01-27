#ifndef _TESTING_H_
#define _TESTING_H_

#include "Demo.hpp"

#include <MemeCore/MemoryManager.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace
{
	struct Test final
	{
		uint64_t	 index;
		const char * name;
		const char * tag;

		Test()
			: index	(NULL)
			, name	(NULL)
			, tag	(NULL)
		{}

		inline friend std::ostream & operator<<(std::ostream & out, const Test & value)
		{
			return (out) 
				<< "{ " 
				<< "[" << value.index << "]"
				<< " | "
				<< "\"" << value.name << "\""
				<< " | "
				<< "\"" << value.tag << "\""
				<< " }";
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace
{
	inline static int32_t testMemoryManager()
	{
		// Data
		/* * * * * * * * * * * * * * * * * * * * */
		enum : size_t 
		{ 
			A, 
			B, 
			C, 
			D, 
			E, 
			F,
			MAX 
		};

		static const std::string Tags[MAX] =
		{
			"A",
			"B",
			"C",
			"D",
			"E",
			"F",
		};

		static Test * test[MAX] = { NULL };


		// Initialize
		/* * * * * * * * * * * * * * * * * * * * */
		ml::Debug::Log("Testing Memory");
		ml::Debug::Log("Max Bytes: {0}", ML_MAX_BYTES);
		ml::Debug::Log("Chunk Size: {0}", sizeof(ml::Chunk));
		ml::Debug::Log("Test Struct Size: {0}", sizeof(Test));
		ml::Debug::out() << std::endl;


		ml::byte data[ML_MAX_BYTES];
		if (!ML_Memory.prime(data, ML_MAX_BYTES))
		{
			return ml::Debug::LogError("Failed priming Memory Manager")
				|| ml::Debug::pause(EXIT_FAILURE);
		}

		// Allocate
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Test *)ML_Memory.allocate(sizeof(Test)))
			{
				test[i]->index	= i;
				test[i]->name	= "Test Name";
				test[i]->tag	= Tags[i].c_str();
				ml::Debug::Log("Allocation Success: {0}", (*test[i]));
			}
			else
			{
				ml::Debug::LogError("Allocation Failure: {0}", (Tags[i]));
			}
		}
		ml::Debug::out() << std::endl << ML_Memory << std::endl;


		// Free
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (ML_Memory.free(test[i]))
			{
				ml::Debug::Log("Free Success: {0}", (test[i]));
			}
			else
			{
				ml::Debug::LogError("Free Failure: {0}", (test[i]));
			}
		}
		ml::Debug::out() << std::endl << ML_Memory << std::endl;

		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t runTests(uint32_t id)
	{
		switch (id)
		{
		case 1: return testMemoryManager();
		}
		return EXIT_FAILURE;
	}
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TESTING_H_
