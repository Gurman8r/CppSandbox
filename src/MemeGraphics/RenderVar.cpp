#include <MemeGraphics/RenderVar.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderVar::RenderVar()
		: m_type(NULL)
		, m_data(NULL)
	{
	}

	RenderVar::RenderVar(const uint32_t type, const_reference data)
		: m_type(type)
		, m_data(data)
	{
	}

	RenderVar::RenderVar(const RenderVar & copy)
		: m_type(copy.m_type)
		, m_data(copy.m_data)
	{
	}

	RenderVar::~RenderVar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}