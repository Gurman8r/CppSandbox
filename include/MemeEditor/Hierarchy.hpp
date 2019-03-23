#ifndef _ML_HIERARCHY_HPP_
#define _ML_HIERARCHY_HPP_

#include <MemeEditor/WindowDrawer.hpp>

#define ML_Hierarchy ml::Hierarchy::getInstance()

namespace ml
{
	class ML_EDITOR_API Hierarchy final
		: public WindowDrawer
		, public ISingleton<Hierarchy>
	{
		friend ISingleton<Hierarchy>;

	private:
		Hierarchy();
		~Hierarchy();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(CString title, bool * p_open) override;

	private:

	};
}

#endif // !_ML_HIERARCHY_HPP_