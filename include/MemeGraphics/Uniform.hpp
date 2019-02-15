#ifndef _UNIFORM_HPP_
#define	_UNIFORM_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	class ML_GRAPHICS_API Uniform final
		: public ITrackable
	{
	public:
		enum Type : int32_t
		{
			None,
			Int,
			Float,
			Vec2,
			Vec3,
			Vec4,
			Mat3,
			Mat4,
			Tex2D,
			MAX_TYPE
		};

	public:
		Uniform();
		Uniform(const String & name);
		Uniform(const String & name, const int32_t type);
		Uniform(const String & name, const int32_t type, const void * data);
		Uniform(const Uniform & copy);
		~Uniform();

	public:
		String		name;
		int32_t		type;
		const void *data;

	public:
		template <class T>
		inline const T * get_pointer() const
		{
			const T * temp;
			assert(
				(temp = reinterpret_cast<const T *>(data)) && 
				("Cannot reinterpret uniform data"));
			return temp;
		}

		template <class T>
		inline const T & get_value() const
		{
			return (*get_pointer<T>());
		}

	public:
		inline operator String() const { return name;	}
		inline operator bool()	 const { return !name.empty(); }
	};
}

#endif // !_UNIFORM_HPP_
