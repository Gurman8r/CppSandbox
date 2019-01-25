#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <MemeCore/ITrackable.h>

namespace ml
{
	struct ML_CORE_API Chunk final
	{
		size_t	size;
		bool	free;
		Chunk *	next;
		Chunk *	prev;
		uint8_t	npos[1];

		Chunk()
			: size(0)
			, free(true)
			, next(NULL)
			, prev(NULL)
		{
			(*npos) = NULL;
		}

		inline friend std::ostream & operator<<(std::ostream & out, const Chunk & value)
		{
			return (out)
				<< "[ Chunk ]"
				<< " { size: " << (value.size)
				<< " | free: " << (value.free ? "true" : "false")
				<< " | next: " << (value.next ? "Good" : "NULL")
				<< " | prev: " << (value.prev ? "Good" : "NULL")
				<< " | npos: " << (value.npos ? (*value.npos) : NULL)
				<< " }";
		}
	};
}

#endif // !_CHUNK_H_
