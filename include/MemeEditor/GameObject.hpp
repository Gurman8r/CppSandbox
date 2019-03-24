#ifndef _ML_GAME_OBJECT_HPP_
#define _ML_GAME_OBJECT_HPP_

#include <MemeEditor/GUI.hpp>
#include <MemeCore/File.hpp>

namespace ml
{
	class ML_EDITOR_API GameObject
		: public ITrackable
	{
	public:
		using Name = typename char[64];

	public:
		GameObject();
		GameObject(CString name);
		GameObject(const GameObject & copy);
		~GameObject();

	public:
		char name[64];
		Transform transform;
		vec4f color;
	};
}

#endif // !_ML_GAME_OBJECT_HPP_