#ifndef _ML_CONFIG_HPP_
#define _ML_CONFIG_HPP_

// Github project URL
/* * * * * * * * * * * * * * * * * * * */
#define ML_PROJECT_URL	"https://www.github.com/Gurman8r/Cppsandbox"


// Disable safety.
/* * * * * * * * * * * * * * * * * * * */
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


//	Configuration Macro
/* * * * * * * * * * * * * * * * * * * */
# if defined(_DEBUG)
#	define ML_DEBUG
# endif


//	C/C++ Macro
/* * * * * * * * * * * * * * * * * * * */
# if defined(__cplusplus)
#	define ML_CPP
# endif


//	Operating System Macro
/* * * * * * * * * * * * * * * * * * * */
# if defined(_WIN64) || defined(_WIN32)
#	define ML_SYSTEM_WINDOWS
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	ifdef near
#		undef near
#	endif
#	ifdef far
#		undef far
#	endif
# elif defined(__APPLE__) && defined(__MACH__)
#	define ML_SYSTEM_APPLE
# elif defined(__unix__)
#	define ML_SYSTEM_UNIX
#	if defined(__ANDROID__)
#		define ML_SYSTEM_ANDROID
#	elif defined(__linux__)
#		define ML_SYSTEM_LINUX
#	elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)	
#		define ML_SYSTEM_FREEBSD
#	else
#		error This UNIX operating system does not support memes.
#	endif
# else
#	error This operating system does not support memes.
# endif


//	MsBuild / GCC Macro
/* * * * * * * * * * * * * * * * * * * */
# ifdef ML_SYSTEM_WINDOWS
# 	ifdef _MSC_VER
#		define ML_MSB _MSC_VER
# 	endif
# else
# 	ifdef __GNUC__
#		define ML_GCC __GNUC__
# 	endif
# endif


//	Platform Macro
/* * * * * * * * * * * * * * * * * * * */
# if defined(ML_SYSTEM_WINDOWS)
#	if defined(_WIN64)
#		define ML_x64
#	else
#		define ML_x86
#	endif
# elif defined(ML_GCC)
#	if defined(__x86_64__) || defined(__ppc64__)
#		define ML_x64
#	else
#		define ML_x86
#	endif
# else
#	error The target platform does not support memes.
# endif


//	Export / Import Macro
/* * * * * * * * * * * * * * * * * * * */
# ifndef ML_STATIC
#	ifdef ML_SYSTEM_WINDOWS
#		define ML_API_EXPORT __declspec(dllexport)
#		define ML_API_IMPORT __declspec(dllimport)
#		ifdef ML_MSB
#			pragma warning(disable: 4031)
#			pragma warning(disable: 4099)
#			pragma warning(disable: 4251)
#		endif
#	else
#		if ML_GCC >= 4
#			define ML_API_EXPORT __attribute__ ((__visibility__ ("default")))
#			define ML_API_IMPORT __attribute__ ((__visibility__ ("default")))
#		else
#			define ML_API_EXPORT
#			define ML_API_IMPORT
#		endif
#	endif
# else
#	define ML_API_EXPORT
#	define ML_API_IMPORT
# endif


#endif // !_ML_CONFIG_HPP_