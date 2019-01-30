#ifndef _LEXER_H_
#define _LEXER_H_

#include <MemeScript/TokenList.h>

namespace ml
{
	class ML_SCRIPT_API Lexer final
		: public ITrackable
	{
	public:
		using CharBuffer = std::vector<char>;
		using const_iterator = CharBuffer::const_iterator;

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

		CharBuffer	getBuffer() const;
		String getString() const;

		Lexer&		clearBuffer();
		Lexer&		setBuffer(const String& value);
		Lexer&		setBuffer(const CharBuffer& value);

		TokenList	splitTokens() const;

		bool scanName(const_iterator& it, String& text) const;
		bool scanNumber(const_iterator& it, String& text) const;
		bool scanString(const_iterator& it, String& text) const;
		bool scanSymbol(const_iterator& it, String& text) const;

		inline Flags getFlags() const { return m_flags; }
		inline void	 setFlags(Flags value) { m_flags = value; }

	private:
		Flags		m_flags;
		CharBuffer	m_buffer;
	};
}

#endif // !_LEXER_H_
