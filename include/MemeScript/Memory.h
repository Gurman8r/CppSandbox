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
		using ScopeMap = std::unordered_map<int32_t, VarMap*>;

	public:
		Memory();
		~Memory();

		std::ostream& display(std::ostream& out) const;

		void	clean();

		bool	delVar(int32_t index, const std::string & name);
		Var *	getVar(int32_t index, const std::string & name) const;
		Var *	newVar(int32_t index, const std::string & name, const Var & value);
		Var *	setVar(int32_t index, const std::string & name, const Var & value);

		const VarMap*	values(int32_t index) const;
		VarMap*			values(int32_t index);
		VarMap*			makeScope(int32_t index);
		bool			clearScope(int32_t index);

	private:
		ScopeMap * m_values;
	};
}

#endif // !_MEMORY_H_
