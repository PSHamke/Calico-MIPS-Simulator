#pragma once

#define  CL_ENABLE_ASSERTS

#ifdef CL_ENABLE_ASSERTS
#define CL_CORE_ASSERT(x, ...) {if(!(x)) {CL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else 
#define CL_CORE_ASSERT(x, ...)
#endif

#define BIT(X) 1<<X
