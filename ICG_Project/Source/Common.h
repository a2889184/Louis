#pragma once

#if _WIN32 || _WIN64
	#if _WIN64
		#define ENVIRONMENT64
	#else
		#define ENVIRONMENT32
	#endif
#endif

#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define ENVIRONMENT64
	#else
		#define ENVIRONMENT32
	#endif
#endif

#if !(_WIN32 || _WIN64 || __GNUC__)
	#define ENVIRONMENT64
#endif

#ifdef ENVIRONMENT64
	#define int8 char
	#define uint8 unsigned char
	#define int32 int
	#define uint32 unsigned int

	#define float32 float
	#define float64 double

	#define boolean bool
#else
	#define int8 char
	#define uint8 unsigned char
	#define int32 int
	#define uint32 unsigned int

	#define float32 float
	#define float64 double

	#define boolean bool
#endif

#ifdef _MSC_VER
	#define thread_local_var __declspec(thread)
#else
	#define thread_local_var __thread
#endif

#define TRUE true
#define FALSE false


#include <iostream>
#include <string>

namespace math
{
	class Vector3f;
}

extern std::ostream& operator<<(std::ostream &stream, const math::Vector3f &r);
