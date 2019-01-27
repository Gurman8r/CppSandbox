#ifndef _TESTING_H_
#define _TESTING_H_

#include "Demo.hpp"

#include <MemeCore/ML_Memory.h>
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
		{
		}
		Test(uint64_t index, const char * name, const char * tag)
			: index	(index)
			, name	(name)
			, tag	(tag)
		{
		}
		Test(const Test & copy)
			: Test(copy.index, copy.name, copy.tag)
		{
		}

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
		ml::Debug::Log("Test Size: {0}", sizeof(Test));
		ml::cout << std::endl;

		enum : size_t { MaxBytes = 4096 };
		ml::byte data[MaxBytes];

		if (!ML_Memory.prime(data, MaxBytes))
		{
			return ml::Debug::LogError("Failed priming Memory Manager")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
		ml::cout << std::endl << ML_Memory << std::endl;


		// Allocate
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Test *)ML_Memory.allocate(sizeof(Test)))
			{
				test[i]->index = i;
				test[i]->name = "Test Name";
				test[i]->tag = Tags[i].c_str();
				ml::Debug::Log("Allocation Success: {0}", (*test[i]));
			}
			else
			{
				ml::Debug::LogError("Allocation Failure: {0}", (Tags[i]));
			}
		}
		ml::cout << std::endl << ML_Memory << std::endl;


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
		ml::cout << std::endl << ML_Memory << std::endl;


		// Allocate
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Test *)ML_Memory.allocate(sizeof(Test)))
			{
				test[i]->index = i;
				test[i]->name = "Test Name";
				test[i]->tag = Tags[i].c_str();
				ml::Debug::Log("Allocation Success: {0}", (*test[i]));
			}
			else
			{
				ml::Debug::LogError("Allocation Failure: {0}", (Tags[i]));
			}
		}
		ml::cout << std::endl << ML_Memory << std::endl;



		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t testMemoryC()
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
		ml::Debug::Log("Block Size: {0}", sizeof(Block));
		ml::Debug::Log("byte* Size: {0}", sizeof(ml::byte *));
		ml::Debug::Log("Test Size: {0}", sizeof(Test));
		ml::cout << std::endl;

		enum : size_t { MaxBytes = 4096 };
		ml::byte data[MaxBytes];

		if (!ml_prime(data, MaxBytes))
		{
			return ml::Debug::LogError("Failed priming Memory Manager")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
		ml::cout << std::endl;
		ml_displayMemory();
		ml::cout << std::endl;


		// Allocate
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Test *)ml_allocate(sizeof(Test)))
			{
				test[i]->index = i;
				test[i]->name = "Test Name";
				test[i]->tag = Tags[i].c_str();
				ml::Debug::Log("Allocation Success: {0}", (*test[i]));
			}
			else
			{
				ml::Debug::LogError("Allocation Failure: {0}", (Tags[i]));
			}
		}
		ml::cout << std::endl;
		ml_displayMemory();
		ml::cout << std::endl;


		// Free
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (ml_free(test[i]))
			{
				ml::Debug::Log("Free Success: {0}", (test[i]));
			}
			else
			{
				ml::Debug::LogError("Free Failure: {0}", (test[i]));
			}
		}
		ml::cout << std::endl;
		ml_displayMemory();
		ml::cout << std::endl;


		// Allocate
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Test *)ml_allocate(sizeof(Test)))
			{
				test[i]->index = i;
				test[i]->name = "Test Name";
				test[i]->tag = Tags[i].c_str();
				ml::Debug::Log("Allocation Success: {0}", (*test[i]));
			}
			else
			{
				ml::Debug::LogError("Allocation Failure: {0}", (Tags[i]));
			}
		}
		ml::cout << std::endl;
		ml_displayMemory();
		ml::cout << std::endl;



		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t testManualMemory1()
	{
		enum : uint32_t { 
			MaxBytes	= 4096,
			ChunkSize	= sizeof(ml::Chunk),
			ByteSize	= sizeof(ml::byte *),
			TestSize	= sizeof(Test),
		};

		ml::Debug::Log("sizeof(Chunk): {0}", ChunkSize);
		ml::Debug::Log("sizeof(byte*): {0}", ByteSize);
		ml::Debug::Log("sizeof(Test) : {0}", TestSize);
		ml::Debug::Log("Data Size : {0}", ChunkSize + ByteSize + TestSize);
		ml::Debug::Log("Max Bytes: {0}", MaxBytes);


		ml::Debug::Log("Prime");
		ml::byte data[MaxBytes];
		if (!ML_Memory.prime(data, MaxBytes))
		{
			return ml::Debug::LogError("Failed Priming Memory")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
		ml::cout << std::endl << ML_Memory << std::endl;


		ml::Debug::Log("Allocate");
		Test * ta = ML_Memory.allocate<Test>();
		Test * tb = ML_Memory.allocate<Test>();
		Test * tc = ML_Memory.allocate<Test>();
		ml::cout << std::endl << ML_Memory << std::endl;


		ml::Debug::Log("Write");
		(*ta) = { 1, "Test", "A" };
		(*tb) = { 2, "Test", "B" };
		(*tc) = { 3, "Test", "C" };

		ml::cout 
			<< (*ta) << std::endl
			<< (*tb) << std::endl
			<< (*tc) << std::endl
			<< std::endl;

		ml::Debug::Log("Read");
		ml::Chunk * ca = ML_Memory.readChunk(ta);
		ml::Chunk * cb = ML_Memory.readChunk(tb);
		ml::Chunk * cc = ML_Memory.readChunk(tc);

		ml::cout << (*((Test *)ca->data)) << std::endl;
		ml::cout << (*((Test *)cb->data)) << std::endl;
		ml::cout << (*((Test *)cc->data)) << std::endl;

		ML_Memory.serializeChunk(ml::cout, (*ML_Memory.readChunk(ta)));
		ML_Memory.serializeChunk(ml::cout, (*ML_Memory.readChunk(tb)));
		ML_Memory.serializeChunk(ml::cout, (*ML_Memory.readChunk(tc)));

		ml::cout << std::endl;


		ml::Debug::Log("Free");

		ML_Memory.free(ca->data);
		ml::cout << std::endl << ML_Memory << std::endl;
		
		ML_Memory.free(cb->data);
		ml::cout << std::endl << ML_Memory << std::endl;
		
		ML_Memory.free(cc->data);
		ml::cout << std::endl << ML_Memory << std::endl;

		
		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t testManualMemory2()
	{
		enum : uint32_t
		{
			MaxBytes = 4096,
			ChunkSize = sizeof(ml::Chunk),
			ByteSize = sizeof(ml::byte *),
			TestSize = sizeof(Test),
		};

		ml::Debug::Log("sizeof(Chunk): {0}", ChunkSize);
		ml::Debug::Log("sizeof(byte*): {0}", ByteSize);
		ml::Debug::Log("sizeof(Test) : {0}", TestSize);
		ml::Debug::Log("Struct Size : {0}", ChunkSize + ByteSize);
		ml::Debug::Log("Total Size : {0}", ChunkSize + ByteSize + TestSize);


		ml::Debug::Log("Prime");
		ml::byte data[MaxBytes];
		if (!ML_Memory.prime(data, MaxBytes))
		{
			return ml::Debug::LogError("Failed Priming Memory")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
		ml::cout << ML_Memory << std::endl;



		ml::Debug::Log("Allocate");

		void * testA = ML_Memory.allocate(512);

		ml::cout << ML_Memory << std::endl;


		ml::Debug::Log("Read");

		ml::Chunk * chunkA = ML_Memory.readChunk(testA);

		ML_Memory.serializeChunk(ml::cout, (*chunkA));

		ml::cout << std::endl;



		ml::Debug::Log("Free");

		ML_Memory.free(chunkA->data);

		ml::cout << ML_Memory << std::endl;


		return ml::Debug::pause(EXIT_SUCCESS);
	}
}


/* * * * * * * * * * * * * * * * * * * * */

namespace
{
	inline static int32_t runTests(uint32_t id)
	{
		switch (id)
		{
		case 1: return testMemoryManager();
		case 2: return testMemoryC();
		case 3: return testManualMemory1();
		case 4: return testManualMemory2();
		}
		return EXIT_FAILURE;
	}
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TESTING_H_
