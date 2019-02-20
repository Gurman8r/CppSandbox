# CppSandbox

Projects
__________
	- Demo
    		- Executable with demos for each library
	- MemeAudio
			- Audio library (OpenAL)
  	- MemeCore
			- Core Library
			- Required by all other projects
    		- Contains classes and structures required by ALL others libraries
	- MemeEditor
			- GUI/Editor Library
			- ML ImGui Implementation
			- 
  	- MemeGraphics
    		- Graphics library (glew)
			- Requires MemeWindow
	- MemeNet
    		- Network library (RakNet)
  	- MemePhysics
    		- Physics library
	- MemeScript
    		- Scripting/Interpreter library
  	- MemeWindow
    		- Window library (glfw)

Help
__________
- Solution Not Building?
	- Retarget the solution to a different Windows SDK version.
	- Run the OpenAL SDK installer "/thirdparty/tools/oalinst.exe"
	- Try updating your graphics drivers.


Docs
__________
	- https://github.com/Gurman8r/CppSandbox/tree/master/docs
	- https://docs.google.com/document/d/1AGI_7HTSVHGOlaVBpLVQISlTbUbD0dpTw3lY-CR2HMA/edit?usp=sharing	
	- https://i.imgur.com/gpnXgHE.jpg