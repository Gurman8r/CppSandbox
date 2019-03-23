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
		GameObject();
		~GameObject();

	public:
		Transform transform;

	};
}

#endif // !_ML_GAME_OBJECT_HPP_