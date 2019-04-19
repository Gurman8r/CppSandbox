#include <MemeGraphics/RenderVar.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderVar::RenderVar()
		: IHandle(NULL)
		, m_type(NULL)
	{
	}

	RenderVar::RenderVar(const uint32_t type, const int64_t data)
		: IHandle(data)
		, m_type(type)
	{
	}

	RenderVar::RenderVar(const RenderVar & copy)
		: IHandle(copy)
		, m_type(copy.m_type)
	{
	}

	RenderVar::~RenderVar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}