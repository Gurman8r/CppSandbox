#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <MemeScript/TokenList.hpp>

namespace ml
{
	class ML_SCRIPT_API Lexer final
		: public ITrackable
	{
	public:
		using const_iterator = typename List<char>::const_iterator;

		enum Flags : uint16_t
		{
			None = 0,
		};
		inline friend Flags operator|(const Lexer::Flags& lhs, const Lexer::Flags& rhs)
		{
			return static_cast<Lexer::Flags>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs));
		}

	public:
		Lexer();
		~Lexer();

		List<char>	getBuffer() const;
		String		getString() const;

		Lexer &		clearBuffer();
		Lexer &		setBuffer(const String & value);
		Lexer &		setBuffer(const List<char> & value);

		TokenList	splitTokens() const;

		bool scanName(const_iterator& it, String& text) const;
		bool scanNumber(const_iterator& it, String& text) const;
		bool scanString(const_iterator& it, String& text) const;
		bool scanSymbol(const_iterator& it, String& text) const;

		inline Flags getFlags() const { return m_flags; }
		inline void	 setFlags(Flags value) { m_flags = value; }

	private:
		Flags		m_flags;
		List<char>	m_buffer;
	};
}

#endif // !_LEXER_HPP_
