#ifndef _UNIFORM_HPP_
#define	_UNIFORM_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IComparable.hpp>

namespace ml
{
	class ML_GRAPHICS_API Uniform final
		: public ITrackable
		, public IComparable<Uniform>
	{
	public:
		enum : int32_t
		{
			None,
			Int,
			Float,
			Vec2,
			Vec3,
			Vec4,
			Mat3,
			Mat4,
			Tex,
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
		String		 name;
		int32_t		 type;
		const void * data;

	public:
		template <class T>
		inline const T * get_pointer() const
		{
			return reinterpret_cast<const T *>(data);
		}

		template <class T>
		inline const T & get_value() const
		{
			if (const T * p = get_pointer<T>())
			{
				return (*get_pointer<T>());
			}
			static const T temp = T();
			return temp;
		}

	public:
		bool good() const;
		
		inline operator bool() const { return good(); }

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

		bool equals(const Uniform & other) const override;
		bool lessThan(const Uniform & other) const override;
	};
}

#endif // !_UNIFORM_HPP_
