#ifndef _ML_EDITOR_GUI_HPP_
#define _ML_EDITOR_GUI_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeGraphics/Transform.hpp>

namespace ml
{
	class ML_EDITOR_API EditorGUI final
	{
	public:
		static void InputMat4f(CString label, mat4f & value);
		static void InputMat3f(CString label, mat3f & value);
		static void InputVec2f(CString label, vec2f & value);
		static void InputVec3f(CString label, vec3f & value);
		static void InputVec4f(CString label, vec4f & value);
	};
}

#endif // !_ML_EDITOR_GUI_HPP_