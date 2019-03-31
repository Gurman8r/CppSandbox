#include <MemeEditor/GameObject.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeGraphics/Color.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GameObject::GameObject(CString name)
		: dirty(false)
		, color(Color::White)
		, model(NULL)
		, shader(NULL)
		, texture(NULL)
	{
		strcpy(this->name, name);
	}

	GameObject::GameObject(const GameObject & copy)
		: dirty(copy.dirty)
		, color(copy.color)
	{
		strcpy(this->name, copy.name);
	}

	GameObject::~GameObject()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}