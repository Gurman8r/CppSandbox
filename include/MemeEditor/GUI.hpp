#ifndef _ML_GUI_HPP_
#define _ML_GUI_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeGraphics/Transform.hpp>

namespace ml
{
	class ML_EDITOR_API GUI final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */

		static bool DrawLabel(CString label);

		/* * * * * * * * * * * * * * * * * * * * */

		static bool EditVec2f(CString label, vec2f & value, float speed = 1.f);
		static bool EditVec3f(CString label, vec3f & value, float speed = 1.f);
		static bool EditVec4f(CString label, vec4f & value, float speed = 1.f);
		static bool EditMat3f(CString label, mat3f & value, float speed = 1.f);
		static bool EditMat4f(CString label, mat4f & value, float speed = 1.f);

		/* * * * * * * * * * * * * * * * * * * * */

		static bool Begin(CString label, bool * p_open, int32_t flags);
		static void End();

		template <
			typename _Fun
		> inline static bool DrawFun(CString label, bool * p_open, int32_t flags, _Fun fun)
		{
			bool good;
			if (good = Begin(label, p_open, flags))
			{
				fun();
			}
			End();
			return good;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_GUI_HPP_