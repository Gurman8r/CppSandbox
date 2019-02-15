#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

#include <MemeCore/Maths.hpp>
#include <MemeCore/STD.hpp>

namespace ml
{
	class ML_CORE_API Random final
	{
	public:
		static void seed();
		static void seed(const uint32_t value);

	public:
		template <typename T>
		inline T roll()
		{
			return static_cast<T>(rand());
		}

		template <typename T>
		inline T roll(const T max)
		{ 
			return roll<T>() / max; 
		}

		template <typename T>
		inline T roll(const T min, const T max)
		{
			return min + (roll<T>(max) * (max - min)); 
		}
	};
}

#endif // !_RANDOM_HPP_
