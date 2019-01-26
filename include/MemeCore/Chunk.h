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
		}

		inline bool mergePrev()
		{
			if (this->prev && (this->prev)->free)
			{
				(this->prev)->size += this->size + sizeof(Chunk);

				(this->prev)->next = this->next;

				if (this->next)
				{
					(this->next)->prev = this->prev;
					
					return true;
				}
			}
			return false;
		}

		inline bool mergeNext()
		{
			if (this->next && (this->next)->free)
			{
				this->size += (this->next)->size + sizeof(Chunk);

				this->next = (this->next)->next;

				if ((this->next)->next)
				{
					((this->next)->next)->prev = this;

					return true;
				}
			}
			return false;
		}

		inline friend std::ostream & operator<<(std::ostream & out, const Chunk & value)
		{
			return (out)
				<< "[ Chunk ]"
				<< " { size: " << (value.size)
				<< " | free: " << (value.free ? "Yes" : "No ")
				<< " | next: " << (value.next ? "Yes" : "No ")
				<< " | prev: " << (value.prev ? "Yes" : "No ")
				<< " | npos: " << (value.npos ? (*value.npos) : NULL)
				<< " }";
		}
	};
}

#endif // !_CHUNK_H_
