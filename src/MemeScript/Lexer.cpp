#include <MemeScript/Lexer.hpp>
#include <MemeScript/StringUtility.hpp>

namespace
{
	inline bool issymbol(const char& c)
	{
		return
			(c >= '!'  && c <= '/') ||
			(c >= ':'  && c <= '@') ||
			(c >= '['  && c <= '`') ||
			(c >= '{'  && c <= '~');
	}
}

namespace ml
{
	Lexer::Lexer()
	{
	}

	Lexer::~Lexer()
	{
	}


	List<char> Lexer::getBuffer() const
	{
		return m_buffer;
	}

	String Lexer::getString() const
	{
		String out;
		for (const char& c : m_buffer)
			out += c;
		return out;
	}

	Lexer & Lexer::clearBuffer()
	{
		m_buffer.clear();
		return (*this);
	}

	Lexer & Lexer::setBuffer(const String & value)
	{
		m_buffer.clear();
		for (auto it = value.begin(); it != value.end(); it++)
		{
			m_buffer.push_back(*it);
		}
		return (*this);
	}

	Lexer& Lexer::setBuffer(const List<char> & value)
	{
		m_buffer = value;
		return (*this);
	}


	Token Lexer::makeToken(const String & value) const
	{
		if (StringUtility::IsInt(value))
			return Token('i', value);

		if (StringUtility::IsDecimal(value))
			return Token('f', value);

		if (StringUtility::IsText(value))
			return Token('s', value);

		if (StringUtility::IsName(value))
			return Token('n', value);

		return Token('\0');
	}

	TokenList Lexer::splitTokens() const
	{
		TokenList tokens;

		List<char>::const_iterator it;
		for (it = m_buffer.begin(); it != m_buffer.end(); it++)
		{
			String text;

			// End of line
			if (*it == '\n')
			{
				tokens.push_back({ TokenType::TOK_ENDL });
			}
			// String
			else if (scanString(it, text))
			{
				tokens.push_back({ TokenType::TOK_STR, text });
			}
			// Name
			else if (scanName(it, text))
			{
				tokens.push_back({ TokenType::TOK_NAME, text });
			}
			// Number
			else if (scanNumber(it, text))
			{
				// Integer
				if (StringUtility::IsInt(text))
				{
					tokens.push_back({ TokenType::TOK_INT, text });
				}
				// Float
				else if (StringUtility::IsDecimal(text))
				{
					tokens.push_back({ TokenType::TOK_FLT, text });
				}
				// Error
				else
				{
					tokens.push_back({ TokenType::TOK_ERR, text });
				}
			}
			// Symbols
			else if (scanSymbol(it, text))
			{
				Token::SymbolMap::const_iterator it = Token::Symbols.find(text);

				if (it != Token::Symbols.end())
				{
					tokens.push_back({ it->second, text });
				}
				else
				{
					tokens.push_back({ TokenType::TOK_ERR, text });
				}
			}
		}

		// End of file
		//tokens.push_back({ TokenType::TOK_ENDF });

		return tokens;
	}


	bool Lexer::scanName(const_iterator & it, String & text) const
	{
		if (isalpha(*it) || (*it) == '_')
		{
			String out;

			while (it != m_buffer.end())
			{
				if (isalnum(*it) || (*it) == '_')
				{
					out += *it;
					it++;
				}
				else
				{
					break;
				}
			}

			it--;
			text = out;
			return true;
		}

		text = String();
		return false;
	}

	bool Lexer::scanNumber(const_iterator& it, String & text) const
	{
		if (isdigit(*it))
		{
			String out;

			while (it != m_buffer.end())
			{
				if (isdigit(*it) || *it == '.')
				{
					out += *it;
					it++;
				}
				else
				{
					break;
				}
			}

			it--;
			text = out;
			return true;
		}

		text = String();
		return false;
	}

	bool Lexer::scanString(const_iterator& it, String & text) const
	{
		if (*it == '\"')
		{
			String out;

			while (++it != m_buffer.end())
			{
				if (*it == '\"')
				{
					break;
				}
				else if (isprint(*it))
				{
					out += *it;
				}
			}

			text = out;
			return true;
		}

		text = String();
		return false;
	}

	bool Lexer::scanSymbol(const_iterator & it, String & text) const
	{
		if (issymbol(*it))
		{
			text = String(1, *it);
			return true;
		}

		text = String();
		return false;
	}




}