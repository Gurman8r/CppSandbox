#ifndef _INSPECTOR_HPP_
#define _INSPECTOR_HPP_

#include <MemeEditor/ResourceManager.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/Transform.hpp>

#define ML_Inspector ml::Inspector::getInstance()

namespace ml
{
	// Property / Object Editor
	class ML_EDITOR_API Inspector
		: public ITrackable
		, public ISingleton<Inspector>
	{
		friend class ISingleton<Inspector>;
		
	private:
		Inspector();
		~Inspector();

	public:
		void ShowHelpMarker(CString desc) const;
		void ShowFramerate() const;

	public:
		Transform & InputTransform(CString label, Transform & value) const;
		mat4f &		InputMat4f(CString label, mat4f & value) const;
		mat3f &		InputMat3f(CString label, mat3f & value) const;
		vec2f &		InputVec2f(CString label, vec2f & value) const;
		vec3f &		InputVec3f(CString label, vec3f & value) const;
		vec4f &		InputVec4f(CString label, vec4f & value) const;
	};
}

#endif // !_INSPECTOR_HPP_
