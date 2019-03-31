#ifndef _ML_GAME_OBJECT_HPP_
#define _ML_GAME_OBJECT_HPP_

#include <MemeEditor/GUI.hpp>
#include <MemeCore/File.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/Model.hpp>

namespace ml
{
	class ML_EDITOR_API GameObject
		: public ITrackable
		, public ITransformable
	{
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
		vec4f		color;
		UniformSet	uniforms;

		const Model *	model;
		const Shader *	shader;
	};
}

#endif // !_ML_GAME_OBJECT_HPP_