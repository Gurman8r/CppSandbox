#ifndef _EDITOR_HPP_
#define _EDITOR_HPP_

#include <MemeEditor/ResourceManager.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/Transform.hpp>

#define ML_Editor ml::Editor::getInstance()

namespace ml
{
	class ML_EDITOR_API Editor
		: public ITrackable
		, public ISingleton<Editor>
	{
		friend class ISingleton<Editor>;
		
	private:
		Editor();
		~Editor();

	public:
		void ShowHelpMarker(CString desc) const;
		void ShowFramerate() const;

	public:
		CString FormatName(CString label) const;

	public:
		Transform & InputTransform(CString label, Transform & value) const;
		mat4f &	InputMat4f(CString label, mat4f & value) const;
		mat3f &	InputMat3f(CString label, mat3f & value) const;
		vec2f &	InputVec2f(CString label, vec2f & value) const;
		vec3f &	InputVec3f(CString label, vec3f & value) const;
		vec4f &	InputVec4f(CString label, vec4f & value) const;

	public:
		inline const bool	open() const	{ return m_open; }
		inline bool &		open()			{ return m_open; }

	private:
		bool m_open;
	};
}

#endif // !_EDITOR_HPP_
