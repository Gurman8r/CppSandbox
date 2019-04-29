#ifndef _ML_UNIFORM_HPP_
#define	_ML_UNIFORM_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IComparable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Uniform final
		: public ITrackable
		, public IComparable<Uniform>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */

		enum : int32_t
		{
			None,
			Int,
			Float,
			Vec2,
			Vec3,
			Vec4, Col4,
			Mat3,
			Mat4,
			Tex2D,
			MAX_TYPE
		};

		/* * * * * * * * * * * * * * * * * * * * */

		Uniform();
		Uniform(const String & name);
		Uniform(const String & name, const int32_t type);
		Uniform(const String & name, const int32_t type, const void * data, int32_t flag = 0);
		Uniform(const Uniform & copy, const void * data, int32_t flag = 0);
		Uniform(const Uniform & copy);
		~Uniform();

		/* * * * * * * * * * * * * * * * * * * * */

		String	name;
		int32_t	type;
		void *	data;
		int32_t	flag;

		/* * * * * * * * * * * * * * * * * * * * */

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

		/* * * * * * * * * * * * * * * * * * * * */

		bool equals(const Uniform & other) const override;
		bool lessThan(const Uniform & other) const override;

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		inline const T * get_ptr() const
		{
			return reinterpret_cast<const T *>(data);
		}

		template <class T>
		inline const T & get_value() const
		{
			return (*get_ptr<T>());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UNIFORM_HPP_