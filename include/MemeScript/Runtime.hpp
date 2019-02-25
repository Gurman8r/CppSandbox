#ifndef _RUNTIME_HPP_
#define _RUNTIME_HPP_

#include <MemeScript/Operator.hpp>
#include <MemeScript/Var.hpp>

namespace ml
{
	class AST_Node;

	class ML_SCRIPT_API Runtime final
		: public ITrackable
	{
		friend class Interpreter;

	public:
		using VarMap	= Map<String, Var *>;
		using ScopeMap	= HashMap<int32_t, VarMap*>;

	public:
		std::ostream& display(std::ostream& out) const;

		void	clean();

		bool	delVar(int32_t index, const String & name);
		Var *	getVar(int32_t index, const String & name) const;
		Var *	newVar(int32_t index, const String & name, const Var & value);
		Var *	setVar(int32_t index, const String & name, const Var & value);

		const VarMap *	values(int32_t index) const;
		VarMap *		values(int32_t index);
		VarMap *		makeScope(int32_t index);
		bool			clearScope(int32_t index);

	private:
		Runtime();
		~Runtime();

		ScopeMap * m_values;
	};
}

#endif // !_RUNTIME_HPP_
