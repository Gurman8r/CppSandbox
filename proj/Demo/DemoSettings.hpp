#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

#include <INIReader.h>
#include <MemeCore/FileSystem.hpp>
#include <MemeWindow/Window.hpp>

// Singleton to store program properties/settings for Demo (INIReader)
#define SETTINGS DEMO::DemoSettings::getInstance()

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	class DemoSettings final
		: public ml::ITrackable
		, public ml::IReadable
		, public ml::ISingleton<DemoSettings>
	{	
		friend class ml::ISingleton<DemoSettings>;

	public:
		ml::String	configINI;		// path to config INI

	public:
		// [General]
		/* * * * * * * * * * * * * * * * * * * * */
		ml::String	assetPath;		// Solution Dir
		ml::String	projectURL;		// Project Github URL
		bool		escapeIsExit;	// Escape key is Exit?
		bool		enableThreads;	// Enable threads?
		ml::String	manifest;		// Name of manifest file
		ml::String	imguiIni;		// Name of imgui INI

		// [Script]
		/* * * * * * * * * * * * * * * * * * * * */
		ml::String	bootScript;		// Script to run on start
		bool		flagShowToks;	// Show Script Tokens
		bool		flagShowTree;	// Show Script Syntax Tree
		bool		flagShowItoP;	// Show Script Infix to Postfix

		// [Window]
		/* * * * * * * * * * * * * * * * * * * * */
		ml::String	title;			// Window Title
		uint32_t	width;			// Window Width
		uint32_t	height;			// Window Height
		uint32_t	bitsPerPixel;	// Bits Per Pixel
		uint32_t	style;
		uint32_t	majorVersion;	// Major Version
		uint32_t	minorVersion;	// Minor Version
		uint32_t	depthBits;		// Depth Bits
		uint32_t	stencilBits;	// Stencil Bits
		uint32_t	profile;		// Profile
		bool		multisample;	// Multisample
		bool		srgbCapable;	// sRGB Capable


		// [Graphics]
		/* * * * * * * * * * * * * * * * * * * * */
		bool		glErrorPause;	// OpenGL Error Pause?
		bool		glExperimental;	// GLEW Experimental?
		float		fieldOfView;	// Field of View
		float		perspNear;		// Persp Near Clipping Range 
		float		perspFar;		// Persp Far  Clipping Range 
		float		orthoNear;		// Ortho Near Clipping Range 
		float		orthoFar;		// Ortho Far  Clipping Range

		// [Audio]
		/* * * * * * * * * * * * * * * * * * * * */
		bool		isMuted;		// Is Muted?

		// [Network]
		/* * * * * * * * * * * * * * * * * * * * */
		bool		isServer;		// Is Server?
		bool		isClient;		// Is Client?


	public:
		inline bool loadFromFile(const ml::String & value) override
		{
			INIReader ini(configINI = value);
			if (ini.ParseError() != EXIT_SUCCESS)
			{
				return false;
			}

			// [General]
			/* * * * * * * * * * * * * * * * * * * * */
			assetPath		= ini.Get("General", "assetPath", "../../../");
			projectURL		= ini.Get("General", "projectURL", "");
			escapeIsExit	= ini.GetBoolean("General", "escapeIsExit", true);
			enableThreads	= ini.GetBoolean("General", "enableThreads", false);
			manifest		= ini.Get("General", "manifest", "manifest.txt");
			imguiIni		= ini.Get("General", "imguiIni", "");

			// [Script]
			/* * * * * * * * * * * * * * * * * * * * */
			bootScript		= ini.Get("Script", "bootScript", "/scripts/hello.meme");
			flagShowToks	= ini.GetBoolean("Script", "flagShowToks", false);
			flagShowTree	= ini.GetBoolean("Script", "flagShowTree", false);
			flagShowItoP	= ini.GetBoolean("Script", "flagShowItoP", false);

			// [Window]
			/* * * * * * * * * * * * * * * * * * * * */
			title			= ini.Get("Window", "title", "MemeLib");
			width			= ini.GetInteger("Window", "width", 640);
			height			= ini.GetInteger("Window", "height", 480);
			bitsPerPixel	= ini.GetInteger("Window", "bitsPerPixel", 32);
			style			= ini.GetInteger("Window", "style", ml::Window::Default);
			majorVersion	= ini.GetInteger("Window", "majorVersion", 3);
			minorVersion	= ini.GetInteger("Window", "minorVersion", 3);
			depthBits		= ini.GetInteger("Window", "depthBits", 24);
			stencilBits		= ini.GetInteger("Window", "stencilBits", 8);
			profile			= ini.GetInteger("Window", "profile", ml::Context::Compat);
			multisample		= ini.GetBoolean("Window", "multisample", false);
			srgbCapable		= ini.GetBoolean("Window", "srgbCapable", false);

			// [Graphics]	
			/* * * * * * * * * * * * * * * * * * * * */
			glErrorPause	= ini.GetBoolean("Graphics", "glErrorPause", false);
			glExperimental	= ini.GetBoolean("Graphics", "glExperimental", true);
			fieldOfView		= (float)ini.GetReal("Graphics", "fieldOfView", 90.0);
			perspNear		= (float)ini.GetReal("Graphics", "perspNear", 0.1);
			perspFar		= (float)ini.GetReal("Graphics", "perspFar", 1000.0);
			orthoNear		= (float)ini.GetReal("Graphics", "orthoNear", -1.0);
			orthoFar		= (float)ini.GetReal("Graphics", "orthoFar", +1.0);

			// [Audio]
			/* * * * * * * * * * * * * * * * * * * * */
			isMuted			= ini.GetBoolean("Audio", "isMuted", false);

			// [Network]
			/* * * * * * * * * * * * * * * * * * * * */
			isServer		= ini.GetBoolean("Network", "isServer", false);
			isClient		= ini.GetBoolean("Network", "isClient", false);

			return true;
		}

		inline bool cleanup() override { return true; }

	public:
		inline ml::vec2u	 winSize()	const { return { width, height }; }
		inline ml::VideoMode video()	const { return { winSize(), bitsPerPixel }; }
		inline ml::Context	 context()	const
		{
			return {
				majorVersion,
				minorVersion,
				profile,
				depthBits,
				stencilBits,
				multisample,
				srgbCapable
			};
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SETTINGS_HPP_
