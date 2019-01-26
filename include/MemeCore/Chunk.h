#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <MemeCore/Export.h>
#include <iostream>

namespace ml
{
	struct ML_CORE_API Chunk final
	{
		size_t	size;
		bool	free;
		Chunk *	prev;
		Chunk *	next;
		uint8_t	npos[1];

		Chunk();

		bool mergePrev();
		bool mergeNext();

		friend std::ostream & operator<<(std::ostream & out, const Chunk & c);
	};
}

#endif // !_CHUNK_H_
