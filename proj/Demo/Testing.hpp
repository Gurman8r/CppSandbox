#ifndef _TESTING_H_
#define _TESTING_H_

#include "Demo.hpp"
#include <MemeCore/MemoryPool.h>
#include <MemeCore/Chunk.h>

// Test Struct
/* * * * * * * * * * * * * * * * * * * * */
namespace
{
	struct Test final
	{
		uint64_t	index;
		const char* name;

		inline friend std::ostream & operator<<(std::ostream & out, const Test & value)
		{
			return (out) << "{ " << value.index << ", " << value.name << " }";
		}
	};
}


// Tests
/* * * * * * * * * * * * * * * * * * * * */
#define MAX_POOL 256

namespace
{
	inline static int32_t testStackMalloc()
	{
		ml::Debug::Log("Testing Stack Malloc");

		static uint8_t	pool[MAX_POOL];
		static size_t	index;

		auto stack_malloc = [](size_t size)
		{
			return ((size + index) > MAX_POOL)
				? NULL
				: &pool[index += size];
		};

		ml::Debug::out()
			<< "Index: " << index << std::endl
			<< "Size:  " << sizeof(Test) << std::endl
			<< std::endl;

		if (Test * test = (Test *)stack_malloc(sizeof(Test)))
		{
			test->index = 1;
			test->name = "Foo";
			ml::Debug::out()
				<< "Value: " << (*test) << std::endl
				<< "Index: " << index << std::endl
				<< std::endl;
		}

		if (Test * test = (Test *)stack_malloc(sizeof(Test)))
		{
			test->index = 2;
			test->name = "Bar";
			ml::Debug::out()
				<< "Value: " << (*test) << std::endl
				<< "Index: " << index << std::endl
				<< std::endl;
		}

		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t testMemoryPool()
	{
		ml::Debug::Log("Testing Memory Pool");

		ml::BytePool<MAX_POOL> pool;
		if (pool.initialize(NULL))
		{
			ml::Debug::out()
				<< "Index: " << pool.index() << std::endl
				<< "Size:  " << pool.size()  << std::endl
				<< "Step:  " << sizeof(Test) << std::endl
				<< std::endl;

			if (Test * test = pool.allocate<Test>())
			{
				test->index = 1;
				test->name = "Foo";
				ml::Debug::out()
					<< "Value: " << (*test) << std::endl
					<< "Index: " << pool.index() << std::endl
					<< std::endl;
			}

			if (Test * test = (Test *)pool.allocate(sizeof(Test)))
			{
				test->index = 2;
				test->name = "Bar";
				ml::Debug::out()
					<< "Value: " << (*test) << std::endl
					<< "Index: " << pool.index() << std::endl
					<< std::endl;
			}

			if (Test * test = (Test *)pool.allocate(sizeof(Test)))
			{
				test->index = 3;
				test->name = "FooBar";
				ml::Debug::out()
					<< "Value: " << (*test) << std::endl
					<< "Index: " << pool.index() << std::endl
					<< std::endl;
			}

			return ml::Debug::pause(EXIT_SUCCESS);
		}
		return ml::Debug::LogError("Failed Initializing Pool")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	inline static int32_t testChunks()
	{
		ml::Debug::Log("Testing Chunks");

		ml::BytePool<MAX_POOL> pool;
		if (pool.initialize(NULL))
		{
			if (ml::Chunk * chunk = pool.allocate<ml::Chunk>())
			{
				ml::Debug::Log("{0}", (*chunk));
			}
		}
		return ml::Debug::pause(EXIT_SUCCESS);
	}

	inline static int32_t runTests(int32_t index)
	{
		switch (index)
		{
		case 0: return testStackMalloc();
		case 1: return testMemoryPool();
		case 2: return testChunks();
		}
		return EXIT_FAILURE;
	}
}

/* * * * * * * * * * * * * * * * * * * * */


#endif // !_TESTING_H_
