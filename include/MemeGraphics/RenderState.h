#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

#include <MemeGraphics/Shader.h>
#include <MemeGraphics/Mesh.h>

namespace ml
{
	struct ML_GRAPHICS_API RenderState final
		: public ITrackable
	{
		RenderState();
		RenderState(const RenderState & copy);

		const Texture * texture;
		const Mesh *	mesh;
		const Shader *	shader;
		vec4f			color;
		Transform		transform;
		uint32_t		textureHandle;
	};
}

#endif // !_RENDER_STATE_H_
