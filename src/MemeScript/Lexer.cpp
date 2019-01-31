#include <MemeScript/Lexer.h>
#include <MemeCore/StringUtility.h>

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


	Lexer::CharBuffer Lexer::getBuffer() const
	{
		return m_buffer;
	}

	string Lexer::getString() const
	{
		string out;
		for (const char& c : m_buffer)
			out += c;
		return out;
	}

	Lexer & Lexer::clearBuffer()
	{
		m_buffer.clear();
		return (*this);
	}

	Lexer & Lexer::setBuffer(const string & value)
	{
		m_buffer.clear();
		for (auto it = value.begin(); it != value.end(); it++)
		{
			m_buffer.push_back(*it);
		}
		return (*this);
	}

	Lexer& Lexer::setBuffer(const CharBuffer & value)
	{
		m_buffer = value;
		return (*this);
	}


	TokenList Lexer::splitTokens() const
	{
		TokenList tokens;

		CharBuffer::const_iterator it;
		for (it = m_buffer.begin(); it != m_buffer.end(); it++)
		{
			string text;

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


	bool Lexer::scanName(const_iterator & it, string & text) const
	{
		if (isalpha(*it))
		{
			string out;

			while (it != m_buffer.end())
			{
				if (isalnum(*it))
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

		text = string();
		return false;
	}

	bool Lexer::scanNumber(const_iterator& it, string & text) const
	{
		if (isdigit(*it))
		{
			string out;

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

		text = string();
		return false;
	}

	bool Lexer::scanString(const_iterator& it, string & text) const
	{
		if (*it == '\"')
		{
			string out;

			while (++it != m_buffer.end())
			{
				//if (*it == '\\')
				//{
				//	continue;
				//}
				//else if (*(it - 1) != '\\' && *it == '\"')

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

		text = string();
		return false;
	}

	bool Lexer::scanSymbol(const_iterator & it, string & text) const
	{
		if (issymbol(*it))
		{
			text = string(1, *it);
			return true;
		}

		text = string();
		return false;
	}




}