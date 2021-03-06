#include <MemeScript/Lexer.hpp>
#include <MemeCore/StringUtility.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Lexer::Lexer()
	{
	}

	Lexer::~Lexer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Token Lexer::genToken(const String & value) const
	{
		if (value.empty())
		{
			return { Token::Empty, value };
		}
		else if (StringUtility::IsInt(value))
		{
			return { Token::Int, value };
		}
		else if (StringUtility::IsDecimal(value))
		{
			return { Token::Float, value };
		}
		else
		{
			return { Token::Str, value };
		}
	}

	TokenList Lexer::genArgsArray(const Args & value) const
	{
		TokenList out;
		out.push_back('[');
		if (!value.empty())
		{
			TokenList temp = genTokenList(value.ToString());

			for (size_t i = 0, imax = temp.size(); i < imax; i++)
			{
				if (temp[i].type == Token::Name)
				{
					temp[i] = { Token::Str, temp[i].data };
				}

				out.push_back(temp[i]);

				if (i < imax - 1)
				{
					out.push_back(',');
				}
			}
		}
		out.push_back(']');
		return out;
	}


	TokenList Lexer::genTokenList(const String & value) const
	{
		return genTokenList(List<char>(value.begin(), value.end()));
	}

	TokenList Lexer::genTokenList(const List<char> & value) const
	{
		TokenList out;

		List<char>::const_iterator it;
		for (it = value.begin(); it != value.end(); it++)
		{
			String text;

			// End of line
			if ((*it) == '\n')
			{
				out.push_back({ '\n' });
			}
			// String
			else if (scanString(value, it, text))
			{
				out.push_back({ Token::Str, text });
			}
			// Name
			else if (scanName(value, it, text))
			{
				out.push_back({ Token::Name, text });
			}
			// Number
			else if (scanNumber(value, it, text))
			{
				// Integer
				if (StringUtility::IsInt(text))
				{
					out.push_back({ Token::Int, text });
				}
				// Float
				else if (StringUtility::IsDecimal(text))
				{
					out.push_back({ Token::Float, text });
				}
				// Error
				else
				{
					out.push_back({ Token::Error, text });
				}
			}
			// Symbols
			else if (scanSymbol(value, it, text))
			{
				Token::SymbolMap::const_iterator it;
				if ((it = Token::Symbols.find(text)) != Token::Symbols.end())
				{
					out.push_back({ it->second, text });
				}
				else
				{
					out.push_back({ Token::Error, text });
				}
			}
		}

		return out;
	}

	TokenTree Lexer::genTokenTree(const TokenList & value) const
	{
		TokenTree tree	= { TokenList() };
		
		for (TokenList::const_iterator it = value.begin(); it != value.end(); it++)
		{
			switch (it->type)
			{
			case '\n': // New Line
				continue;

			case '#': // Comment
				while ((it != value.end()) && ((*it) != '\n')) { it++; }
				break;

			case '@': // Preprocessor
			{
				String line;
				while ((it != value.end()) && ((*it) != '\n')) 
				{
					line += it->data;
					it++; 
				}
				line.erase(line.begin());
				if (ML_Interpreter.execCommand(line).isErrorType())
				{
					Debug::logError("Lexer : Failed executing preprocessor command \"{0}\"", 
						line);
				}
			}
			break;

			case '{': // Begin Block
				tree.back().push_back(*it);

			case ';': // End Statement
				if (!tree.back().empty())
				{
					tree.push_back(TokenList());
				}
				break;

			default: // Other
			{
				tree.back().push_back(*it);
			}
			break;
			}
		}

		return tree;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Lexer::scanName(const List<char> & value, const_iterator & it, String & text) const
	{
		if (isalpha(*it) || (*it) == '_')
		{
			String out;
			while (it != value.end())
			{
				if (isalnum(*it) || (*it) == '_')
				{
					out += (*it);
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

	bool Lexer::scanNumber(const List<char> & value, const_iterator & it, String & text) const
	{
		if (isdigit(*it))
		{
			String out;
			while (it != value.end())
			{
				if (isdigit(*it) || (*it) == '.')
				{
					out += (*it);
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

	bool Lexer::scanString(const List<char> & value, const_iterator & it, String & text) const
	{
		if ((*it) == '\"')
		{
			String out;
			while (++it != value.end())
			{
				if ((*it) == '\"')
				{
					break;
				}
				else if (isprint(*it))
				{
					out += (*it);
				}
			}
			text = out;
			return true;
		}
		text = String();
		return false;
	}

	bool Lexer::scanSymbol(const List<char> & value, const_iterator & it, String & text) const
	{
		auto issymbol = [](const char c)
		{
			return
				(c >= '!'  && c <= '/') ||
				(c >= ':'  && c <= '@') ||
				(c >= '['  && c <= '`') ||
				(c >= '{'  && c <= '~');
		};

		if (issymbol(*it))
		{
			text = String(1, (*it));
			return true;
		}
		text = String();
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}