#include <MemeEditor/GameObject.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeGraphics/Color.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GameObject::GameObject()
		: GameObject("New Game Object")
	{
	}

	GameObject::GameObject(CString name)
		: color(Color::White)
	{
		strcpy(this->name, name);
	}

	GameObject::GameObject(const GameObject & copy)
		: GameObject(copy.name)
	{
	}

	GameObject::~GameObject()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}