#ifndef _ML_MEMORY_H_
#define _ML_MEMORY_H_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.h>
#include <inttypes.h>
#include <stddef.h>

/* * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CORE_API
	#define ML_CORE_API
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
	size_t			size; // Size of data
	bool			free; // Is free?
	struct Block *	prev; // Previous node in list
	struct Block *	next; // Next node in list

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


	// Utility
	/* * * * * * * * * * * * * * * * * * * * */
	ML_CORE_API bool	ml_prime(byte * data, size_t size);
	ML_CORE_API size_t	ml_increment(size_t size);
	ML_CORE_API bool	ml_mergeBlockPrev(Block * value);
	ML_CORE_API bool	ml_mergeBlockNext(Block * value);
	ML_CORE_API bool	ml_splitBlock(Block * value, size_t size);
	
	ML_CORE_API void	ml_displayMemory();
	ML_CORE_API bool	ml_displayBlock(Block * value);

	ML_CORE_API struct Block * ml_createBlock(size_t size);
	ML_CORE_API struct Block * ml_findEmptyBlock(size_t size);
	ML_CORE_API struct Block * ml_findBlockByValue(void * value);

#ifdef ML_CPP
}
#endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MEMORY_H_