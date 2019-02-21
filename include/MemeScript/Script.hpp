#ifndef _SCRIPT_HPP_
#define _SCRIPT_HPP_

#include <MemeScript/AST_Block.hpp>
#include <MemeCore/File.hpp>

namespace ml
{
	class ML_SCRIPT_API Script
		: public ITrackable
		, public IReadable
	{
	public:
		Script();
		~Script();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		bool run();

		const Var & ret() const { return m_ret; }

	private:
		File		m_file;
		TokenList	m_tokens;
		AST_Block *	m_ast;
		Var			m_ret;
	};
}

#endif // !_SCRIPT_HPP_
