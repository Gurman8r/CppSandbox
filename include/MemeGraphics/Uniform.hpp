#ifndef _UNIFORM_HPP_
#define	_UNIFORM_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	struct ML_GRAPHICS_API Uniform final
		: public ITrackable
	{
		enum ID : uint16_t
		{
			Model,
			View,
			Proj,
			Color,
			Texture,
			Curve,
			MAX_UNIFORM_ID
		};
		static const String Names[Uniform::MAX_UNIFORM_ID];

		Uniform();
		Uniform(const ID id);
		Uniform(const String & name);
		Uniform(const char * name);
		Uniform(const Uniform & copy);
		~Uniform();

		String name;

		inline operator String	() const { return name; }
		inline operator bool	() const { return !name.empty(); }
	};
}

#endif // !_UNIFORM_HPP_
