#pragma once

#ifdef DLL_EXPORT
#define DLL_API __declspec(dllexport)
#elif DLL_IMPORT
#define DLL_API __declspec(dllimport)
#else
#define DLL_API
#endif