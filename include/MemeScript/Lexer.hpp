#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <MemeScript/TokenList.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Lexer final
		: public ITrackable
	{
		friend class Interpreter;

	public:
		using const_iterator = typename List<char>::const_iterator;

	public:
		Token		makeToken(const String & value) const;
		TokenList	makeArray(const Args & args);

		TokenList	splitTokens(const String & value) const;
		TokenList	splitTokens(const List<char> & value) const;

		bool scanName(const List<char> & value, const_iterator& it, String & text) const;
		bool scanNumber(const List<char> & value, const_iterator& it, String & text) const;
		bool scanString(const List<char> & value, const_iterator& it, String & text) const;
		bool scanSymbol(const List<char> & value, const_iterator& it, String & text) const;

	private:
		Lexer();
		~Lexer();
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_LEXER_HPP_
