#ifndef _CONTEXT_SETTINGS_H_
#define _CONTEXT_SETTINGS_H_

#include <MemeWindow/Export.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	struct ML_WINDOW_API ContextSettings final
		: public ITrackable
	{
		enum Attribute : uint32_t
		{
			Compat	= 0x00032002, // Compatibility attribute
			Core    = 0x00032001, // Core attribute
			Debug   = 0x00022007  // Debug attribute
		};

		uint32_t	depthBits;
		uint32_t	stencilBits;
		uint32_t	antialiasingLevel;
		uint32_t	majorVersion;
		uint32_t	minorVersion;
		uint32_t	attributeFlags;
		bool		sRgbCapable;

		ContextSettings(
			uint32_t depth = 0,
			uint32_t stencil = 0,
			uint32_t antialiasing = 0,
			uint32_t major = 1,
			uint32_t minor = 1,
			uint32_t attrib = Attribute::Compat,
			bool sRgb = false)
			: depthBits(depth)
			, stencilBits(stencil)
			, antialiasingLevel(antialiasingLevel)
			, majorVersion(major)
			, minorVersion(minor)
			, attributeFlags(attrib)
			, sRgbCapable(sRgb)
		{};

		ContextSettings(const ContextSettings& copy)
			: depthBits(copy.depthBits)
			, stencilBits(copy.stencilBits)
			, antialiasingLevel(copy.antialiasingLevel)
			, majorVersion(copy.majorVersion)
			, minorVersion(copy.minorVersion)
			, attributeFlags(copy.attributeFlags)
			, sRgbCapable(copy.sRgbCapable)
		{};
	};
}

#endif // !_CONTEXT_SETTINGS_H_
