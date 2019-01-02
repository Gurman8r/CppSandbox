#ifndef _AST_BLOCK_H_
#define _AST_BLOCK_H_

#include <MemeScript/AST_Node.h>
#include <MemeScript/Var.h>

namespace ml
{
	struct AST_Func;

	class ML_SCRIPT_API AST_Block
		: public AST_Node
	{
	public:
		using FuncMap = std::unordered_map<std::string, AST_Func*>;

	public:
		AST_Block(const list_type& body);
		~AST_Block();

		virtual std::ostream& display(std::ostream& out) const;

		bool	delv(const Var::name_t & name);
		Var *	getv(const Var::name_t & name) const;
		Var *	newv(const Var::name_t & name, const Var & value);
		Var *	setv(const Var::name_t & name, const Var & value);

		Var *	getRet() const;
		Var *	setRet(const Var & value) const;

		bool		addFunc(const std::string & name, AST_Func * func);
		AST_Func*	getFunc(const std::string & name);
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
