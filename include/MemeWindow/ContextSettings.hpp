#ifndef _ML_CONTEXT_HPP_
#define _ML_CONTEXT_HPP_

#include <MemeWindow/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API ContextSettings final
		: public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum Profile : uint32_t
		{
			Any		= 0,
			Debug   = 0x22007, // Debug	 | 139271
			Core    = 0x32001, // Core	 | 204801
			Compat	= 0x32002, // Compat | 204802
		};

		/* * * * * * * * * * * * * * * * * * * * */
		
		uint32_t	majorVersion;
		uint32_t	profile;
		uint32_t	minorVersion;
		uint32_t	depthBits;
		uint32_t	stencilBits;
		bool		multisample;
		bool		srgbCapable;

		/* * * * * * * * * * * * * * * * * * * * */

		ContextSettings(
			uint32_t		majorVersion	= 1,
			uint32_t		minorVersion	= 1,
			uint32_t		profile			= Profile::Compat,
			uint32_t		depthBits		= 0,
			uint32_t		stencilBits		= 0,
			bool			multisample		= false,
			bool			srgbCapable		= false)
			: majorVersion	(majorVersion)
			, minorVersion	(minorVersion)
			, profile		(profile)
			, depthBits		(depthBits)
			, stencilBits	(stencilBits)
			, multisample	(multisample)
			, srgbCapable	(srgbCapable)
		{};

		ContextSettings(const ContextSettings & copy)
			: majorVersion(	copy.majorVersion)
			, minorVersion(	copy.minorVersion)
			, profile(		copy.profile)
			, depthBits(	copy.depthBits)
			, stencilBits(	copy.stencilBits)
			, multisample(	copy.multisample)
			, srgbCapable(	copy.srgbCapable)
		{};

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTEXT_HPP_