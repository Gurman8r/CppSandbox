#ifndef _ML_RANDOM_HPP_
#define _ML_RANDOM_HPP_

#include <MemeCore/Export.hpp>
#include <MemeCore/StandardLib.hpp>
#include <MemeCore/ISingleton.hpp>

#define ML_Random ml::Random::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Random final
		: public ISingleton<Random>
	{
		friend class ISingleton<Random>;

	public:
		static void seed();
		static void seed(const uint32_t value);

	public:
		template <
			class T
		> static inline T roll()
		{
			return static_cast<T>(std::rand());
		}

		template <
			class T
		> static inline T roll(const T max)
		{ 
			return (roll<T>() / max);
		}

		template <
			class T
		> static inline T roll(const T min, const T max)
		{
			return (min + (roll<T>(max) * (max - min)));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RANDOM_HPP_