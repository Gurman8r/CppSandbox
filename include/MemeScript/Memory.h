#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <MemeScript/Operator.h>
#include <MemeScript/Var.h>
#include <unordered_map>

namespace ml
{
	class AST_Node;

	class ML_SCRIPT_API Memory final
		: public ITrackable
	{
	public:
		using VarMap = std::map<std::string, Var *>;
		using ScopeMap = std::unordered_map<int, VarMap*>;

	public:
		Memory();
		~Memory();

		std::ostream& display(std::ostream& out) const;

		void	clean();

		bool	delVar(int index, const std::string & name);
		Var *	getVar(int index, const std::string & name) const;
		Var *	newVar(int index, const std::string & name, const Var & value);
		Var *	setVar(int index, const std::string & name, const Var & value);

		const VarMap*	values(int index) const;
		VarMap*			values(int index);
		VarMap*			makeScope(int index);
		bool			clearScope(int index);

	private:
		ScopeMap * m_values;
	};
}

#endif // !_MEMORY_H_
