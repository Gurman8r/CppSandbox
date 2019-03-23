#ifndef _ML_HIERARCHY_HPP_
#define _ML_HIERARCHY_HPP_

#include <MemeEditor/Editor.hpp>

#define ML_Hierarchy ml::Hierarchy::getInstance()

namespace ml
{
	class ML_EDITOR_API Hierarchy
		: public Editor
		, public ISingleton<Hierarchy>
	{
		friend ISingleton<Hierarchy>;

	private:
		Hierarchy();
		~Hierarchy();

	public:
		bool draw(CString title, bool * p_open) override;


	};
}

#endif // !_ML_HIERARCHY_HPP_