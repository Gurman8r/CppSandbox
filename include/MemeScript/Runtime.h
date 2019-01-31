#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include <MemeScript/Operator.h>
#include <MemeScript/Var.h>

namespace ml
{
	class AST_Node;

	class ML_SCRIPT_API Runtime final
		: public ITrackable
	{
	public:
		using VarMap	= std::map<String, Var *>;
		using ScopeMap	= std::unordered_map<int32_t, VarMap*>;

	public:
		Runtime();
		~Runtime();

		std::ostream& display(std::ostream& out) const;

		void	clean();

		bool	delVar(int32_t index, const String & name);
		Var *	getVar(int32_t index, const String & name) const;
		Var *	newVar(int32_t index, const String & name, const Var & value);
		Var *	setVar(int32_t index, const String & name, const Var & value);

		const VarMap*	values(int32_t index) const;
		VarMap*			values(int32_t index);
		VarMap*			makeScope(int32_t index);
		bool			clearScope(int32_t index);

	private:
		ScopeMap * m_values;
	};
}

#endif // !_RUNTIME_H_