#ifndef _ML_GUI_TOOL_HPP_
#define _ML_GUI_TOOL_HPP_

#include <MemeEditor/GUI_Base.hpp>

namespace ml
{
	// Base GUI Tool
	class ML_EDITOR_API GUI_Tool
		: public GUI_Base
	{
	public:
		using base_type = typename GUI_Tool;

	public:
		GUI_Tool();
		virtual ~GUI_Tool();

	public:
		virtual bool beginDraw() = 0;
		virtual bool endDraw() = 0;
	};
}

#endif // !_ML_GUI_TOOL_HPP_