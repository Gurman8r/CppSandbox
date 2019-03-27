#ifndef _ML_PROFILER_HPP_
#define _ML_PROFILER_HPP_

#include <MemeEditor/GUI_Window.hpp>

#define ML_Profiler ml::Profiler::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Profiler
		: public GUI_Window
		, public ISingleton<Profiler>
	{
		friend class ISingleton<Profiler>;

	private:
		Profiler();
		~Profiler();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PROFILER_HPP_
