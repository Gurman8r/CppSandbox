#ifndef _ISINGLETON_H_
#define _ISINGLETON_H_

namespace ml
{
	template <typename T>
	class ISingleton
	{
	public:
		static T & getInstance()
		{
			static T instance;
			return instance;
		}

	protected:
		ISingleton() {}
		~ISingleton() {}

	public:
		ISingleton(ISingleton const &) = delete;
		ISingleton & operator=(ISingleton const &) = delete;
	};
}

#endif // !_ISINGLETON_H_
