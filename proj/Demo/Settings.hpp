#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <INIReader.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/ISingleton.h>
#include <MemeCore/DebugUtility.h>

// Singleton to store program properties/settings (INIReader)
#define SETTINGS demo::Settings::getInstance()

namespace demo
{
	class Settings final
		: public ml::ITrackable
		, public ml::ISingleton<Settings>
	{
		friend class ml::ISingleton<Settings>;

	public:
		// [General]
		std::string	rootPath;	// Where's all the data?

		// [Window]
		std::string	winTitle;	// Window Title
		uint32_t winWidth;		// Window Width
		uint32_t winHeight;		// Window Height

		// [Script]
		std::string	scrFile;	// Script to run on start
		bool scrShowToks;		// Show Script Tokens
		bool scrShowTree;		// Show Script Syntax Tree
		bool scrShowItoP;		// Show Script Infix to Postfix

		// [Graphics]
		float fieldOfView;		// Field of View
		float perspNear;		// Persp Near Clipping Range 
		float perspFar;			// Persp Far  Clipping Range 
		float orthoNear;		// Ortho Near Clipping Range 
		float orthoFar;			// Ortho Far  Clipping Range 

		// [Audio]
		bool isMuted;			// Is Muted?

		// [Network]
		bool isServer;			// Is Server?

	public:
		inline bool loadFromFile(const std::string & filename)
		{
			INIReader ini(filename.c_str());
			if (ini.ParseError() == 0)
			{
				// [General]
				rootPath = ini.Get("General", "rootPath", "../../../assets");

				// [Window]
				winTitle = ini.Get("Window", "title", "MemeLib");
				winWidth = ini.GetInteger("Window", "width", 640);
				winHeight = ini.GetInteger("Window", "height", 480);

				// [Script]
				scrFile = ini.Get("Script", "scrFile", "");
				scrShowToks = ini.GetBoolean("Script", "scrShowToks", false);
				scrShowTree = ini.GetBoolean("Script", "scrShowTree", false);
				scrShowItoP = ini.GetBoolean("Script", "scrShowItoP", false);

				// [Graphics]
				fieldOfView = (float)ini.GetReal("Graphics", "fieldOfView", 90.0);
				perspNear = (float)ini.GetReal("Graphics", "perspNear", 0.1);
				perspFar = (float)ini.GetReal("Graphics", "perspFar", 1000.0);
				orthoNear = (float)ini.GetReal("Graphics", "orthoNear", -1.0);
				orthoFar = (float)ini.GetReal("Graphics", "orthoFar", +1.0);

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
