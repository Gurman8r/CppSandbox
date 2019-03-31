#ifndef _ML_GAME_OBJECT_HPP_
#define _ML_GAME_OBJECT_HPP_

#include <MemeEditor/GUI.hpp>
#include <MemeCore/File.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/Model.hpp>

namespace ml
{
	class Hierarchy;

	class ML_EDITOR_API GameObject
		: public ITrackable
	{
		friend class Hierarchy;

	public:
		enum : uint32_t { NameSize = 64U };

		using Name = typename char[NameSize];

	public:
		GameObject(CString name);
		GameObject(const GameObject & copy);
		~GameObject();

	public:
		bool		dirty;
		Name		name;
		Transform	transform;
		vec4f		color;
		UniformSet	uniforms;

		const Model *	model;
		const Shader *	shader;
		const Texture * texture;
	};
}

#endif // !_ML_GAME_OBJECT_HPP_