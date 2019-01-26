#include <MemeCore/Chunk.h>	
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	Chunk::Chunk()
		: size(0)
		, free(true)
		, prev(NULL)
		, next(NULL)
	{
	}

	bool Chunk::mergePrev()
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

	bool Chunk::mergeNext()
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

	std::ostream & operator<<(std::ostream & out, const Chunk & c)
	{
		const std::type_info & info(typeid(c));
		return (out)
		<< FG::White << "[ " << FG::Gray << info.name() << FG::White << " ]"
		<< FG::White << " { " << FG::Green << "size: " << FG::Yellow << (c.size)
		<< FG::White << " | " << FG::Green << "free: " << FG::Yellow << (c.free ? "Yes" : "No ")
		<< FG::White << " | " << FG::Green << "addr: " << FG::Yellow << (&c)
		<< FG::White << " | " << FG::Green << "prev: " << FG::Yellow << (c.prev)
		<< FG::White << " | " << FG::Green << "next: " << FG::Yellow << (c.next)
		<< FG::White << " | " << FG::Green << "npos: " << FG::Yellow << (c.npos ? (*c.npos) : (-1))
		<< FG::White << " }"
		<< FMT();
	}
}