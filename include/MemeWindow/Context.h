#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <MemeWindow/Export.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	struct ML_WINDOW_API Context final
		: public ITrackable
	{
		enum Profile : uint32_t
		{
			Any		= 0,
			Compat	= 0x32002, // Compatibility
			Core    = 0x32001, // Core
			Debug   = 0x22007  // Debug
		};

		uint32_t	majorVersion;
		uint32_t	minorVersion;
		uint32_t	depthBits;
		uint32_t	stencilBits;
		uint32_t	profile;
		bool		multisample;
		bool		sRgbCapable;

		Context(
			uint32_t	majorVersion	= 1,
			uint32_t	minorVersion	= 1,
			uint32_t	depthBits		= 24,
			uint32_t	stencilBits		= 8,
			uint32_t	profile			= Profile::Compat,
			bool		multisample		= false,
			bool		sRgbCapable		= false)
			: majorVersion(majorVersion)
			, minorVersion(minorVersion)
			, depthBits(depthBits)
			, stencilBits(stencilBits)
			, profile(profile)
			, multisample(multisample)
			, sRgbCapable(sRgbCapable)
		{};

		Context(const Context & copy)
			: majorVersion(copy.majorVersion)
			, minorVersion(copy.minorVersion)
			, depthBits(copy.depthBits)
			, stencilBits(copy.stencilBits)
			, profile(copy.profile)
			, multisample(copy.multisample)
			, sRgbCapable(copy.sRgbCapable)
		{};
	};
}

#endif // !_CONTEXT_SETTINGS_H_