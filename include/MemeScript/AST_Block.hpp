#ifndef _AST_BLOCK_HPP_
#define _AST_BLOCK_HPP_

#include <MemeScript/AST_Node.hpp>
#include <MemeScript/Var.hpp>

namespace ml
{
	struct AST_Func;

	// Block
	/* * * * * * * * * * * * * * * * * * * * */
	class ML_SCRIPT_API AST_Block : public AST_Node
	{
	public:
		using FuncMap = HashMap<String, AST_Func*>;

	public:
		AST_Block();
		AST_Block(const list_type& body);
		~AST_Block();

		virtual std::ostream& display(std::ostream& out) const;

		bool	delVar(const String & name);
		Var *	getVar(const String & name) const;
		Var *	newVar(const String & name, const Var & value);
		Var *	setVar(const String & name, const Var & value);

		Var *	getRet() const;
		Var *	setRet(const Var & value) const;

		bool		addFunc(const String & name, AST_Func * func);
		AST_Func*	getFunc(const String & name);
		FuncMap		getFuncs() const;

		AST_Block*	block() const;

		Var	 evaluate() const;
		bool run() override;
		bool runFirst();

	private:
		FuncMap m_funcs;
	};
}

#endif // !_AST_BLOCK_HPP_
