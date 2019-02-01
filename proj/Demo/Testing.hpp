#ifndef _TESTING_HPP_
#define _TESTING_HPP_

#include <MemeCore/MemoryManager.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace demo
{
	struct Foo final
	{
		uint64_t	 index;
		const char * name;
		const char * tag;

		Foo()
			: index(NULL)
			, name(NULL)
			, tag(NULL)
		{
		}
		Foo(uint64_t index, const char * name, const char * tag)
			: index(index)
			, name(name)
			, tag(tag)
		{
		}
		Foo(const Foo & copy)
			: Foo(copy.index, copy.name, copy.tag)
		{
		}

		inline friend std::ostream & operator<<(std::ostream & out, const Foo & value)
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

	struct Bar final
	{
		uint64_t	index;
		int64_t		id;
		int64_t		category;
		const char *name;
		const char *tag;
		const char *desc;

		inline friend std::ostream & operator<<(std::ostream & out, const Bar & value)
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

namespace demo
{
	inline static int32_t testScripts()
	{
		using namespace ml;

		AST_Block program({
			new AST_Print(new AST_String("Hello, World!"), true),
		});

		if (!program.run())
		{
			Debug::LogError("Failed");
		}

		return Debug::pause(EXIT_SUCCESS);
	}

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

		static const ml::String Tags[MAX] =
		{
			"A",
			"B",
			"C",
			"D",
			"E",
			"F",
		};

		static Foo * test[MAX] = { NULL };

		enum : uint32_t
		{
			MaxBytes = 4096,
			ChunkSize = sizeof(ml::Chunk),
			ByteSize = sizeof(ml::byte *),
			FooSize = sizeof(Foo),
			BarSize = sizeof(Bar),
			Offset = ChunkSize + ByteSize
		};

		ml::cout << "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";

		ml::Debug::Log("sizeof(Chunk): {0}", ChunkSize);
		ml::Debug::Log("sizeof(byte*): {0}", ByteSize);
		ml::Debug::Log("Offset: {0}", Offset);
		ml::Debug::Log("Max Bytes: {0}", MaxBytes);
		ml::cout << ml::endl;
		ml::Debug::Log("sizeof(Foo) : {0}", FooSize);
		ml::Debug::Log("sizeof(Bar) : {0}", BarSize);

		ml::cout << "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";

		ml::Debug::Log("Prime");
		ml::byte data[MaxBytes];
		if (!ML_Memory.prime(data, MaxBytes))
		{
			return ml::Debug::LogError("Failed Priming Memory")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
		ml::cout
			<< ml::endl
			<< ML_Memory
			<< ml::endl
			<< "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";


		ml::Debug::Log("Allocate");
		//void * fa = ML_Memory.allocate(MaxBytes - (Offset * FooSize));
		ml::cout
			<< ml::endl
			<< ML_Memory
			<< ml::endl
			<< "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";


		ml::Debug::Log("Free");
		//ML_Memory.free(fa);
		ml::cout
			<< ml::endl
			<< ML_Memory
			<< ml::endl
			<< "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";


		// Allocate
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Foo *)ML_Memory.allocate(sizeof(Foo)))
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
		ml::cout << ml::endl << ML_Memory << ml::endl;


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
		ml::cout << ml::endl << ML_Memory << ml::endl;


		// Allocate
		/* * * * * * * * * * * * * * * * * * * * */
		for (size_t i = 0; i < MAX; i++)
		{
			if (test[i] = (Foo *)ML_Memory.allocate(sizeof(Foo)))
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
		ml::cout << ml::endl << ML_Memory << ml::endl;


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
		ml::cout << ml::endl << ML_Memory << ml::endl;



		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t testManualMemory1()
	{
		enum : uint32_t { 
			MaxBytes	= 4096,
			ChunkSize	= sizeof(ml::Chunk),
			ByteSize	= sizeof(ml::byte *),
			FooSize		= sizeof(Foo),
		};

		ml::Debug::Log("sizeof(Chunk): {0}", ChunkSize);
		ml::Debug::Log("sizeof(byte*): {0}", ByteSize);
		ml::Debug::Log("sizeof(Test) : {0}", FooSize);
		ml::Debug::Log("Data Size : {0}", ChunkSize + ByteSize + FooSize);
		ml::Debug::Log("Max Bytes: {0}", MaxBytes);


		ml::Debug::Log("Prime");
		ml::byte data[MaxBytes];
		if (!ML_Memory.prime(data, MaxBytes))
		{
			return ml::Debug::LogError("Failed Priming Memory")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
		ml::cout << ml::endl << ML_Memory << ml::endl;


		ml::Debug::Log("Allocate");
		Foo * ta = ML_Memory.allocate<Foo>();
		Foo * tb = ML_Memory.allocate<Foo>();
		Foo * tc = ML_Memory.allocate<Foo>();
		ml::cout << ml::endl << ML_Memory << ml::endl;


		ml::Debug::Log("Write");
		(*ta) = { 1, "Test", "A" };
		(*tb) = { 2, "Test", "B" };
		(*tc) = { 3, "Test", "C" };

		ml::cout 
			<< (*ta) << ml::endl
			<< (*tb) << ml::endl
			<< (*tc) << ml::endl
			<< ml::endl;

		ml::Debug::Log("Read");
		ml::Chunk * ca = ML_Memory.readChunk(ta);
		ml::Chunk * cb = ML_Memory.readChunk(tb);
		ml::Chunk * cc = ML_Memory.readChunk(tc);

		ml::cout << (*((Foo *)ca->npos)) << ml::endl;
		ml::cout << (*((Foo *)cb->npos)) << ml::endl;
		ml::cout << (*((Foo *)cc->npos)) << ml::endl;

		ML_Memory.serializeChunk(ml::cout, (*ML_Memory.readChunk(ta)));
		ML_Memory.serializeChunk(ml::cout, (*ML_Memory.readChunk(tb)));
		ML_Memory.serializeChunk(ml::cout, (*ML_Memory.readChunk(tc)));

		ml::cout << ml::endl;


		ml::Debug::Log("Free");

		ML_Memory.free(ca->npos);
		ml::cout << ml::endl << ML_Memory << ml::endl;
		
		ML_Memory.free(cb->npos);
		ml::cout << ml::endl << ML_Memory << ml::endl;
		
		ML_Memory.free(cc->npos);
		ml::cout << ml::endl << ML_Memory << ml::endl;

		
		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t testManualMemory2()
	{
		enum : uint32_t {
			MaxBytes	= 4096,
			ChunkSize	= sizeof(ml::Chunk),
			ByteSize	= sizeof(ml::byte *),
			FooSize		= sizeof(Foo),
			BarSize		= sizeof(Bar),
			Offset		= ChunkSize + ByteSize
		};

		ml::cout << "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";

		ml::Debug::Log("sizeof(Chunk): {0}", ChunkSize);
		ml::Debug::Log("sizeof(byte*): {0}", ByteSize);
		ml::Debug::Log("Offset: {0}", Offset);
		ml::Debug::Log("Max Bytes: {0}", MaxBytes);
		ml::cout << ml::endl;
		ml::Debug::Log("sizeof(Foo) : {0}", FooSize);
		ml::Debug::Log("sizeof(Bar) : {0}", BarSize);

		ml::cout<< "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";

		
		ml::Debug::Log("Prime");
		ml::byte data[MaxBytes];
		if (!ML_Memory.prime(data, MaxBytes) 
			|| !ML_Memory.free(ML_Memory.allocate(MaxBytes - (Offset * 2)))
			)
		{
			return ml::Debug::LogError("Failed Priming Memory")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
		ml::cout
			<< ml::endl
			<< ML_Memory
			<< ml::endl
			<< "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";

		Foo * fa, *fb, *fc, *fd;

		ml::Debug::Log("Allocate");
		fa = ML_Memory.allocate<Foo>();
		fb = ML_Memory.allocate<Foo>();
		fc = ML_Memory.allocate<Foo>();
		fd = ML_Memory.allocate<Foo>();
		ml::cout 
			<< ml::endl 
			<< ML_Memory
			<< ml::endl
			<< "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";
		
		
		ml::Debug::Log("Free");
		ML_Memory.free(fa);
		ML_Memory.free(fb);
		ML_Memory.free(fc);
		ML_Memory.free(fd);
		ml::cout
			<< ml::endl
			<< ML_Memory
			<< ml::endl
			<< "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";

		return ml::Debug::pause(EXIT_SUCCESS);
	}
}

/* * * * * * * * * * * * * * * * * * * * */

namespace demo
{
	inline static int32_t runTests(uint32_t id)
	{
		switch (id)
		{
		case 1: return testScripts();
		case 2: return testMemoryManager();
		case 3: return testManualMemory1();
		case 4: return testManualMemory2();
		}
		return EXIT_FAILURE;
	}
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TESTING_HPP_
