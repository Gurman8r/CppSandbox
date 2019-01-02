#include <MemeScript/Memory.h>
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	Memory::Memory()
		: m_values(new ScopeMap())
	{
		makeScope(0);
	}

	Memory::~Memory()
	{
		clean();
	}


	std::ostream & Memory::display(std::ostream & out) const
	{
		out << FG::White << "Memory:"
			<< std::endl;

		ScopeMap::const_iterator scope;
		for (scope = m_values->begin(); scope != m_values->end(); scope++)
		{
			const int& index = scope->first;

			out << FG::Yellow << "\tScope [" << index << "]"
				<< std::endl;

			if (const VarMap* varMap = scope->second)
			{
				VarMap::const_iterator vp;
				for (vp = varMap->begin(); vp != varMap->end(); vp++)
				{
					out << "\t\t"
						<< FG::White << vp->first << " = " << *vp->second
						<< std::endl;
				}
			}
		}

		return out;
	}


	void Memory::clean()
	{
		for (auto sPair : (*m_values))
		{
			for (auto vPair : (*sPair.second))
			{
				delete vPair.second;
			}

			sPair.second->clear();

			delete sPair.second;
		}
	}


	bool Memory::delVar(int index, const Var::name_t& name)
	{
		if (values(index))
		{
			VarMap::const_iterator it = values(index)->find(name);

			if (it != values(index)->end())
			{
				delete it->second;

				values(index)->erase(it);

				return true;
			}
		}
		return false;
	}

	Var * Memory::getVar(int index, const Var::name_t& name) const
	{
		if (values(index))
		{
			VarMap::const_iterator it = values(index)->find(name);
			if (it != values(index)->end())
			{
				return it->second;
			}
		}
		return NULL;
	}

	Var * Memory::newVar(int index, const Var::name_t & name, const Var & value)
	{
		if (values(index) || makeScope(index))
		{
			VarMap::const_iterator it = values(index)->find(name);

			if (it == values(index)->end())
			{
				Var * v = new Var(value);

				v->setScope(index);

				values(index)->insert({ name, v });

				return v;
			}
		}
		return NULL;
	}

	Var * Memory::setVar(int index, const Var::name_t & name, const Var & value)
	{
		if (Var * v = getVar(index, name))
		{
			v->Set(value);

			return v;
		}
		return newVar(index, name, value);
	}


	const Memory::VarMap * Memory::values(int index) const
	{
		ScopeMap::const_iterator it = m_values->find(index);
		if (it != m_values->end())
		{
			return it->second;
		}
		return NULL;
	}

	Memory::VarMap * Memory::values(int index)
	{
		ScopeMap::iterator it = m_values->find(index);
		if (it != m_values->end())
		{
			return it->second;
		}
		return NULL;
	}


	Memory::VarMap* Memory::makeScope(int index)
	{
		if (!values(index))
		{
			m_values->insert({ index, new VarMap() });

			setVar(index, "?", Var().makeVoid());
		}
		return values(index);
	}

	bool Memory::clearScope(int index)
	{
		if (VarMap* vars = values(index))
		{
			for (auto pair : (*vars))
			{
				delete pair.second;
			}

			m_values->erase(index);

			return true;
		}
		return false;
	}
}