#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <INIReader.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/ISingleton.h>
#include <MemeCore/IResource.h>
#include <MemeCore/DebugUtility.h>
#include <MemeWindow/Context.h>

// Singleton to store program properties/settings (INIReader)
#define SETTINGS demo::Settings::getInstance()

namespace demo
{
	class Settings final
		: public ml::ITrackable
		, public ml::IResource
		, public ml::ISingleton<Settings>
	{	friend class ml::ISingleton<Settings>;
	public:
		// [General]
		std::string	rootPath;		// Where's all the data?
		bool		runTests;		// Run test functions

		// [Script]
		std::string	scrFile;		// Script to run on start
		bool		scrShowToks;	// Show Script Tokens
		bool		scrShowTree;	// Show Script Syntax Tree
		bool		scrShowItoP;	// Show Script Infix to Postfix

		// [Window]
		std::string	title;			// Window Title
		uint32_t	width;			// Window Width
		uint32_t	height;			// Window Height
		uint32_t	bitsPerPixel;	// Bits Per Pixel

		// [Context]
		uint32_t	majorVersion;	// Major Version
		uint32_t	minorVersion;	// Minor Version
		uint32_t	depthBits;		// Depth Bits
		uint32_t	stencilBits;	// Stencil Bits
		uint32_t	profile;		// Profile
		bool		multisample;	// Multisample
		bool		sRgbCapable;	// sRGB Capable

		// [Graphics]
		float		fieldOfView;	// Field of View
		float		perspNear;		// Persp Near Clipping Range 
		float		perspFar;		// Persp Far  Clipping Range 
		float		orthoNear;		// Ortho Near Clipping Range 
		float		orthoFar;		// Ortho Far  Clipping Range

		// [Audio]
		bool		isMuted;		// Is Muted?

		// [Network]
		bool		isServer;		// Is Server?

	public:
		inline bool cleanup() override { return true; }

		inline bool loadFromFile(const std::string & filename) override
		{
			INIReader ini(filename.c_str());
			if (ini.ParseError() == 0)
			{
				// [General]
				rootPath	= ini.Get("General", "rootPath", "../../../assets");
				runTests	= ini.GetBoolean("General", "runTests", false);

				// [Script]
				scrFile		= ini.Get("Script", "scrFile", "");
				scrShowToks = ini.GetBoolean("Script", "scrShowToks", false);
				scrShowTree = ini.GetBoolean("Script", "scrShowTree", false);
				scrShowItoP = ini.GetBoolean("Script", "scrShowItoP", false);

				// [Window]
				title		= ini.Get("Window", "title", "MemeLib");
				width		= ini.GetInteger("Window", "width",			640);
				height		= ini.GetInteger("Window", "height",		480);
				bitsPerPixel= ini.GetInteger("Window", "bitsPerPixel",	32);

				// [Context]
				majorVersion= ini.GetInteger("Context", "majorVersion", 3);
				minorVersion= ini.GetInteger("Context", "minorVersion", 3);
				depthBits	= ini.GetInteger("Context", "depthBits",	24);
				stencilBits = ini.GetInteger("Context", "stencilBits",	8);
				profile		= ini.GetInteger("Context", "profile",		ml::Context::Compat);
				multisample = ini.GetBoolean("Context", "multisample",	false);
				sRgbCapable = ini.GetBoolean("Context", "sRgbCapable",	false);

				// [Graphics]
				fieldOfView = (float)ini.GetReal("Graphics", "fieldOfView", 90.0);
				perspNear	= (float)ini.GetReal("Graphics", "perspNear",	0.1);
				perspFar	= (float)ini.GetReal("Graphics", "perspFar",	1000.0);
				orthoNear	= (float)ini.GetReal("Graphics", "orthoNear",	-1.0);
				orthoFar	= (float)ini.GetReal("Graphics", "orthoFar",	+1.0);

				// [Audio]
				isMuted = ini.GetBoolean("Audio", "isMuted", false);

				// [Network]
				isServer = ini.GetBoolean("Network", "isServer", false);

				return true;
			}
			return ml::Debug::LogError("Failed Loading Settings \"{0}\"", filename);
		}

		inline const std::string pathTo(const std::string & filename) const
		{
			return rootPath + filename;
		}
	};
}

#endif // !_SETTINGS_H_
