#ifndef _ML_RUNTIME_HPP_
#define _ML_RUNTIME_HPP_

#include <MemeScript/Operator.hpp>
#include <MemeScript/Var.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Runtime ml::Runtime::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Runtime final
		: public ITrackable
		, public ISingleton<Runtime>
	{
		friend class ISingleton<Runtime>;

	public:
		using VarMap	= Map<String, Var *>;
		using ScopeMap	= HashMap<int32_t, VarMap*>;

	private:
		Runtime();
		~Runtime();

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
		ScopeMap * m_values;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RUNTIME_HPP_