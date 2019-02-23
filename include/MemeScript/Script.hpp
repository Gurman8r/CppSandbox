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

		bool	reload();
		bool	build(const Args & args);
		bool	run();

	public:
		inline const String		& path() const { return m_path; }
		inline const File		& file() const { return m_file; }
		inline const TokenList	& toks() const { return m_toks; }
		inline const Var		& out()  const { return m_out;  }

	private:
		String		m_path;
		File		m_file;
		TokenList	m_toks;
		AST_Block *	m_tree;
		Var			m_out;
	};
}

#endif // !_SCRIPT_HPP_
