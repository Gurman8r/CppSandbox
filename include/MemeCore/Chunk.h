#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <MemeCore/Export.h>
#include <iostream>

namespace ml
{
	using byte = uint8_t;

	struct ML_CORE_API Chunk final
	{
		size_t	size;	// Size of npos
		bool	free;	// Is free?
		Chunk *	prev;	// Prev chunk in list
		Chunk *	next;	// Next chunk in list
		byte	npos[1];// End of metadata struct, pointer to npos

		Chunk()
			: size(NULL)
			, free(NULL)
			, prev(NULL)
			, next(NULL)
		{
		};
	};
}

#endif // !_CHUNK_H_
