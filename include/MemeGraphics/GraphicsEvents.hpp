#ifndef _ML_GRAPHICS_EVENTS_HPP_
#define _ML_GRAPHICS_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>
#include <MemeGraphics/Export.hpp>
#include <MemeCore/Console.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API GraphicsEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_GRAPHICS_EVENT = IEvent::EV_GRAPHICS + 1,

			EV_GL_Error,

			MAX_GRAPHICS_EVENT
		};

		GraphicsEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API GL_ErrorEvent final : public GraphicsEvent
	{
		const String		& file;
		const uint32_t		& line;
		const CString		& expr;
		const uint32_t		& errorCode;
		const String		& errorName;
		const String		& errorDesc;

		GL_ErrorEvent(
			const String	& file,
			const uint32_t	& line,
			const CString	& expr,
			const uint32_t	& errorCode,
			const String	& errorName,
			const String	& errorDesc)
			: GraphicsEvent	(EV_GL_Error)
			, file			(file)
			, line			(line)
			, expr			(expr)
			, errorCode		(errorCode)
			, errorName		(errorName)
			, errorDesc		(errorDesc)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << FMT()
				<< endl
				<< FG::Red
				<< "An internal OpenGL call failed in " << file << "(" << line << ")"
				<< FG::Yellow << endl << "Code: "
				<< FG::White << endl << "\t" << errorCode
				<< FG::Yellow << endl << "Expression: "
				<< FG::White << endl << "\t" << expr
				<< FG::Yellow << endl << "Description:"
				<< FG::White << endl << "\t" << errorName
				<< FG::White << endl << "\t" << errorDesc
				<< FMT()
				<< endl;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GRAPHICS_EVENTS_HPP_