#include <MemeEditor/GameObject.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeGraphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	GameObject::GameObject(CString name)
		: dirty(false)
		, color(Color::White)
		, model(NULL)
		, shader(NULL)
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

	void GameObject::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (model && shader)
		{
			renderFlags.apply();
			shader->applyUniforms(uniforms);
			shader->bind();
			target.draw(*model);
		}
	}
}