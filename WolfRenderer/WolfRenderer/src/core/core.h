#pragma once


#ifdef WLFR_PLATFORM_WINDOWS

	#ifdef WLFR_BUILD_DLL

		#define WLFR_API __declspec(dllexport)


	#else
		
		#define WLFR_API __declspec(dllimport)

	#endif	
#else

	#error WolfRenderer only supports windows! 

#endif

#ifdef WLFR_ENABLE_ASSERTS
     #define WLFR_ASSERT(x, ...) {    if(!(x)) {WLFR_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();    }     }
     #define WLFR_CORE_ASSERT(x, ...) {    if(!(x)) {WLFR_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();    }     }
#else
     #define WLFR_ASSERT(x, ...)
     #define WLFR_CORE_ASSERT(x, ...)
#endif


//using bitshift left so that multiple events can have a classification of different types

#define BIT(x) (1 << x)

