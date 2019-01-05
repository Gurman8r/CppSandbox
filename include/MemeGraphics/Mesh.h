#ifndef _MESH_H_
#define _MESH_H_

#include <MemeGraphics/VertexArray.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/IResource.h>
namespace ml
{
	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IResource
	{
	public:
		Mesh();
		~Mesh();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

	private:

	};
}

#endif // !_MESH_H_
