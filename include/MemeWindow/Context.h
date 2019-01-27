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
			Debug   = 0x22007, // Debug	 | 139271
			Core    = 0x32001, // Core	 | 204801
			Compat	= 0x32002, // Compat | 204802
		};

		uint32_t	majorVersion;
		uint32_t	minorVersion;
		uint32_t	depthBits;
		uint32_t	stencilBits;
		uint32_t	profile;
		bool		multisample;
		bool		srgbCapable;

		Context(
			uint32_t		majorVersion	= 1,
			uint32_t		minorVersion	= 1,
			uint32_t		depthBits		= 24,
			uint32_t		stencilBits		= 8,
			uint32_t		profile			= Profile::Compat,
			bool			multisample		= false,
			bool			srgbCapable		= false)
			: majorVersion	(majorVersion)
			, minorVersion	(minorVersion)
			, depthBits		(depthBits)
			, stencilBits	(stencilBits)
			, profile		(profile)
			, multisample	(multisample)
			, srgbCapable	(srgbCapable)
		{};

		Context(const Context & copy)
			: majorVersion(	copy.majorVersion)
			, minorVersion(	copy.minorVersion)
			, depthBits(	copy.depthBits)
			, stencilBits(	copy.stencilBits)
			, profile(		copy.profile)
			, multisample(	copy.multisample)
			, srgbCapable(	copy.srgbCapable)
		{};
	};
}

#endif // !_CONTEXT_SETTINGS_H_
