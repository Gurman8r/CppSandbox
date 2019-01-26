#ifndef _TESTING_H_
#define _TESTING_H_

#include "Demo.hpp"
#include <MemeCore/MemoryManager.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace
{
	struct Test final
	{
		const char* name;
		uint64_t	index;

		Test() 
			: index(0)
			, name("") 
		{}

		Test(const Test & copy)
			: index(copy.index)
			, name(copy.name) 
		{}

		inline friend std::ostream & operator<<(std::ostream & out, const Test & value)
		{
			return (out) << "{ [" << value.index << "] : \"" << value.name << "\" }";
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace
{
	inline static int32_t testMemoryManager()
	{
		ml::Debug::Log("Testing Memory Manager");
		ml::Debug::Log("Max Bits: {0}", ml::MemoryManager::Capacity);
		ml::Debug::Log("Chunk Size: {0}", sizeof(ml::Chunk));
		ml::Debug::Log("Test Struct Size: {0}", sizeof(Test));
		ml::Debug::out() << std::endl;


		// Data
		enum : size_t 
		{ 
			A, 
			B, 
			C, 
			//D, 
			//E, 
			//F,
			MAX 
		};

		static const std::string Names[MAX] = 
		{
			"Test A",
			"Test B",
			"Test C",
			//"Test D",
			//"Test E",
			//"Test F",
		};

		static Test * test[MAX] = { NULL };


		// Allocate
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Test *)ML_Memory.allocate(sizeof(Test)))
			{
				test[i]->index = i;
				test[i]->name = Names[i].c_str();
				ml::Debug::Log("Allocation Success | {0}", (*test[i]));
			}
			else
			{
				ml::Debug::LogWarning("Allocation Failure: {0}", Names[i]);
			}
		}

		//ml::Debug::out() << ML_Memory << std::endl;
		//ml::Debug::pause(EXIT_SUCCESS);


		// Free
		for (size_t i = 0; i < MAX; i++)
		{
			if (ML_Memory.free(test[i]))
			{
				ml::Debug::Log("{0} | Free Success", Names[i]);
			}
			else
			{
				ml::Debug::LogWarning("{0} | Free Failure", Names[i]);
			}
		}

		return ml::Debug::pause(EXIT_SUCCESS);
	}
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TESTING_H_
