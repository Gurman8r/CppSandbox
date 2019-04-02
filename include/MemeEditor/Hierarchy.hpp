#ifndef _ML_HIERARCHY_HPP_
#define _ML_HIERARCHY_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeEditor/GameObject.hpp>

#define ML_Hierarchy ml::Hierarchy::getInstance()

namespace ml
{
	class ML_EDITOR_API Hierarchy final
		: public GUI_Window
		, public ISingleton<Hierarchy>
	{
		friend ISingleton<Hierarchy>;

	public:
		using ObjectMap			= HashMap<String, GameObject *>;
		using iterator			= typename ObjectMap::iterator;
		using const_iterator	= typename ObjectMap::const_iterator;

	private:
		Hierarchy();
		~Hierarchy();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;


	public:
		GameObject * getObject(const String & name);
		GameObject * newObject(const GameObject & value);


	private:
		bool edit_object_name(char * buf, GameObject * value);


	public:
		inline const ObjectMap & objects() const { return m_objects; }
		inline ObjectMap & objects() { return m_objects; }

	private:
		struct Funcs;

		ObjectMap m_objects;
	};
}

#endif // !_ML_HIERARCHY_HPP_