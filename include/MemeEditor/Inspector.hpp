#ifndef _ML_INSPECTOR_HPP_
#define _ML_INSPECTOR_HPP_

#include <MemeEditor/GUI_Window.hpp>

#define ML_Inspector ml::Inspector::getInstance()

namespace ml
{
	class ML_EDITOR_API Inspector final
		: public GUI_Window
		, public ISingleton<Inspector>
	{
		friend class ISingleton<Inspector>;

	private:
		Inspector();
		~Inspector();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;
	};
}

#endif // !_ML_INSPECTOR_HPP_