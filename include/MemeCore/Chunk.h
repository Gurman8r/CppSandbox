#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <MemeCore/Export.h>
#include <iostream>

namespace ml
{
	using byte = uint8_t;

	struct ML_CORE_API Chunk final
	{
		size_t	size;
		bool	free;
		Chunk *	prev;
		Chunk *	next;
		byte	npos[1];

		Chunk();

		friend std::ostream & operator<<(std::ostream & out, const Chunk & c);
	};
}

#endif // !_CHUNK_H_
