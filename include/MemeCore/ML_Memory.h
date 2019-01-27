#ifndef _ML_MEMORY_H_
#define _ML_MEMORY_H_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.h>
#include <inttypes.h>
#include <stddef.h>

/* * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CORE_API
	#define ML_CORE_API __declspec(dllexport)
	#ifdef __cplusplus
		#define ML_CPP
	#endif
#endif

/* * * * * * * * * * * * * * * * * * * * */

#ifdef ML_CPP
using byte = uint8_t;
#else
typedef uint8_t byte;
typedef uint8_t bool;
#endif

/* * * * * * * * * * * * * * * * * * * * */

typedef struct Block
{
	size_t			size;	// Size of data
	bool			free;	// Is free?
	struct Block *	prev;	// Previous node in list
	struct Block *	next;	// Next node in list
	byte			npos[1];// End of metadata

} Block;

/* * * * * * * * * * * * * * * * * * * * */

#ifdef ML_CPP
extern "C"
{
#endif

	// Core
	/* * * * * * * * * * * * * * * * * * * * */
	
	ML_CORE_API void *	ml_allocate(size_t size);
	ML_CORE_API bool	ml_free(void * value);
	ML_CORE_API bool	ml_prime(byte * data, size_t size);
	
	ML_CORE_API void	ml_displayMemory();
	ML_CORE_API bool	ml_displayBlock(Block * block);

	// Internal
	/* * * * * * * * * * * * * * * * * * * * */

	ML_CORE_API bool	ml_isValidBlock(Block * block);

	ML_CORE_API struct Block * ml_writeBlock(size_t addr, size_t size);
	ML_CORE_API struct Block * ml_appendBlock(size_t size);

	ML_CORE_API struct Block * ml_findBlock(size_t size);
	ML_CORE_API struct Block * ml_readBlock(void * addr);

	ML_CORE_API struct Block * ml_mergeBlockPrev(Block * value);
	ML_CORE_API struct Block * ml_mergeBlockNext(Block * value);
	ML_CORE_API struct Block * ml_splitBlock(Block * value, size_t size);

	/* * * * * * * * * * * * * * * * * * * * */

#ifdef ML_CPP
}
#endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MEMORY_H_