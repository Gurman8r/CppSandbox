#ifndef _AST_BLOCK_H_
#define _AST_BLOCK_H_

#include <MemeScript/AST_Node.hpp>
#include <MemeScript/Var.hpp>

namespace ml
{
	struct AST_Func;

	class ML_SCRIPT_API AST_Block
		: public AST_Node
	{
	public:
		using FuncMap = std::unordered_map<String, AST_Func*>;

	public:
		AST_Block();
		AST_Block(const list_type& body);
		~AST_Block();

		virtual std::ostream& display(std::ostream& out) const;

		bool	delv(const String & name);
		Var *	getv(const String & name) const;
		Var *	newv(const String & name, const Var & value);
		Var *	setv(const String & name, const Var & value);

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

#endif // !_AST_BLOCK_H_
