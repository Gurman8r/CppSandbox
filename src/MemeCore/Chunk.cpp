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

	std::ostream & operator<<(std::ostream & out, const Chunk & c)
	{
		const std::type_info & info(typeid(c));
		return (out)
			<< FG::White << "[ " << FG::Gray << info.name() << FG::White << " ]"
			<< FG::White << " { " << FG::Green << "size: " << FG::Yellow << std::setw(3) << (c.size)
			<< FG::White << " | " << (c.free ? FG::Magenta : FG::Cyan) << (c.free ? "free" : "used")
			<< FG::White << " | " << FG::Green << "addr: " << FG::Yellow << (&c)
			<< FG::White << " | " << FG::Green << "prev: " << FG::Yellow << (c.prev)
			<< FG::White << " | " << FG::Green << "next: " << FG::Yellow << (c.next)
			<< FG::White << " | " << FG::Green << "npos: " << FG::Yellow << (&c.npos)
			<< FG::White << " | " << FG::Green << "indx: " << FG::Yellow << ((int)c.npos[0])
			<< FG::White << " }"
			<< FMT();
	}
}