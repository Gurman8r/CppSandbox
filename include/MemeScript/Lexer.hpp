#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <MemeScript/TokenList.hpp>
#include <MemeScript/Args.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Lexer final
		: public ITrackable
	{
	public:
		using const_iterator = typename List<char>::const_iterator;

	public:
		Lexer();
		~Lexer();

	public:
		Token		genToken(const String & value) const;
		TokenList	genArgsArray(const Args & value) const;
		TokenList	genTokenList(const String & value) const;
		TokenList	genTokenList(const List<char> & value) const;
		TokenTree	genTokenTree(const TokenList & value) const;

	public:
		bool scanName(const List<char> & value, const_iterator & it, String & text) const;
		bool scanNumber(const List<char> & value, const_iterator & it, String & text) const;
		bool scanString(const List<char> & value, const_iterator & it, String & text) const;
		bool scanSymbol(const List<char> & value, const_iterator & it, String & text) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_LEXER_HPP_
