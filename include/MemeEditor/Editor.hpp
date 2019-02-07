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
		static void ShowHelpMarker(const char * desc);

	public:
		mat4f & InputMat4f(const char * label, mat4f & value);
		mat3f & InputMat3f(const char * label, mat3f & value);

	public:
		inline const bool	open() const	{ return m_open; }
		inline bool &		open()			{ return m_open; }

	private:
		bool m_open;
	};
}

#endif // !_EDITOR_HPP_
