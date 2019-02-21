#ifndef _SCRIPT_HPP_
#define _SCRIPT_HPP_

#include <MemeScript/Interpreter.hpp>
#include <MemeCore/IReadable.hpp>

namespace ml
{
	class Script
		: public ITrackable
		, public IReadable
	{
	public:
		Script();
		~Script();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

	private:

	};
}

#endif // !_SCRIPT_HPP_
