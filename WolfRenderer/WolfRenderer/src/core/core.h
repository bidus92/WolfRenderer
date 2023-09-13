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

//using bitshift left so that multiple events can have a classification of different types

#define BIT(x) (1 << x)