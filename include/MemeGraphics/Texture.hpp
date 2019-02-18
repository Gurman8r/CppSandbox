#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <MemeCore/Rect.hpp>
#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Image.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_GRAPHICS_API Texture final
		: public ITrackable
		, public IReadable
		, public IHandle<uint32_t>
	{
	public:
		Texture();
		Texture(const Texture & copy);
		~Texture();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & filename, uint32_t type);
		bool loadFromImage(const Image & value);
		bool loadFromImage(const Image & value, uint32_t type);

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		bool update(const Texture & texture);
		bool update(const Image & image);
		bool update(const uint8_t * pixels, const vec2u & size, const vec2u & pos);

	public:
		bool create(const vec2u & size);
		bool create(const vec2u & size, uint32_t type);
		bool create(
			const uint8_t * pixels, 
			const vec2u & size,
			GL::Format	colFormat,
			GL::Format	intFormat, 
			bool smooth,
			bool repeat,
			uint32_t type);

	public:
		Texture & swap(Texture & value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		Texture & generateMipmap();
		Texture & invalidateMipmap();
		
		const Image copyToImage() const;

		inline Texture & operator=(const Texture & value)
		{
			static Texture temp;
			return swap(temp);
		}

	public:
		static void	bind(const Texture * value);

	public:
		inline const uint32_t	type()			const { return m_type; }
		inline const vec2u &	size()			const { return m_size; }
		inline const vec2u &	actualSize()	const { return m_actualSize; }
		inline const GL::Format colFormat()		const { return m_colFormat; }
		inline const GL::Format internalFmt()	const { return m_intFormat; }
		inline const bool		smooth()		const { return m_smooth; }
		inline const bool		repeated()		const { return m_repeated; }
		inline const bool		mipmapped()		const { return m_mipmapped; }
		inline const uint32_t	width()			const { return size()[0]; }
		inline const uint32_t	height()		const { return size()[1]; }

	private:
		uint32_t	m_type;
		vec2u		m_size;
		vec2u		m_actualSize;
		GL::Format	m_colFormat;
		GL::Format	m_intFormat;
		bool		m_smooth;
		bool		m_repeated;
		bool		m_mipmapped;

	};
}

#endif // !_TEXTURE_HPP_
