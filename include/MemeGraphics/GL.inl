#include <MemeGraphics/GL.hpp>

namespace ml
{
	// GL::Attachment Operators
	/* * * * * * * * * * * * * * * * * * * * */
	template <typename T>
	inline GL::Attachment operator+(const GL::Attachment lhs, const T rhs)
	{
		return ML_TYPE_CLAMP(
			GL::Attachment,
			static_cast<uint32_t>(lhs) + static_cast<uint32_t>(rhs),
			GL::ColorAttachment0,
			GL::ColorAttachment15);
	}
	
	template <typename T>
	inline GL::Attachment operator-(const GL::Attachment lhs, const T rhs)
	{
		return (lhs + (-rhs));
	}
	
	template <typename T>
	inline GL::Attachment & operator+=(GL::Attachment & lhs, const T rhs)
	{
		return (lhs = (lhs + rhs));
	}
	
	template <typename T>
	inline GL::Attachment & operator-=(GL::Attachment & lhs, const T rhs)
	{
		return (lhs = (lhs - rhs));
	}
	
	inline GL::Attachment operator++(GL::Attachment & lhs)
	{
		return (lhs += 1);
	}
	
	inline GL::Attachment operator++(GL::Attachment & lhs, int32_t)
	{
		GL::Attachment temp = lhs;
		lhs += 1;
		return temp;
	}
	
	inline GL::Attachment operator--(GL::Attachment & lhs)
	{
		return (lhs += 1);
	}
	
	inline GL::Attachment operator--(GL::Attachment & lhs, int32_t)
	{
		GL::Attachment temp = lhs;
		lhs += 1;
		return temp;
	}


	// GL::TextureID Operators
	/* * * * * * * * * * * * * * * * * * * * */
	template <typename T>
	inline GL::TextureID operator+(const GL::TextureID lhs, const T rhs)
	{
		return ML_TYPE_CLAMP(
			GL::TextureID,
			static_cast<uint32_t>(lhs) + static_cast<uint32_t>(rhs),
			GL::Texture0,
			GL::Texture31);
	}
	
	template <typename T>
	inline GL::TextureID operator-(const GL::TextureID lhs, const T rhs)
	{
		return (lhs + (-rhs));
	}
	
	template <typename T>
	inline GL::TextureID & operator+=(GL::TextureID & lhs, const T rhs)
	{
		return (lhs = (lhs + rhs));
	}
	
	template <typename T>
	inline GL::TextureID & operator-=(GL::TextureID & lhs, const T rhs)
	{
		return (lhs = (lhs - rhs));
	}
	
	inline GL::TextureID operator++(GL::TextureID & lhs)
	{
		return (lhs += 1);
	}
	
	inline GL::TextureID operator++(GL::TextureID & lhs, int32_t)
	{
		GL::TextureID temp = lhs;
		lhs += 1;
		return temp;
	}
	
	inline GL::TextureID operator--(GL::TextureID & lhs)
	{
		return (lhs += 1);
	}
	
	inline GL::TextureID operator--(GL::TextureID & lhs, int32_t)
	{
		GL::TextureID temp = lhs;
		lhs += 1;
		return temp;
	}


	// GL::Mask Operators
	/* * * * * * * * * * * * * * * * * * * * */
	inline GL::Mask operator&(const GL::Mask lhs, const GL::Mask rhs)
	{
		return static_cast<GL::Mask>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}
	
	inline GL::Mask operator|(const GL::Mask lhs, const GL::Mask rhs)
	{
		return static_cast<GL::Mask>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}
	
	inline GL::Mask & operator&=(GL::Mask & lhs, const GL::Mask rhs)
	{
		return (lhs = (lhs & rhs));
	}
	
	inline GL::Mask & operator|=(GL::Mask & lhs, const GL::Mask rhs)
	{
		return (lhs = (lhs | rhs));
	}
}