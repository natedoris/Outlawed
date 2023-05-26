// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Forward DLL Entries to oldhook.dll
#pragma comment(linker, "/export:DLLEntryPoint=oldhook.DLLEntryPoint")
#pragma comment(linker, "/export:HookOff=oldhook.HookOff")
#pragma comment(linker, "/export:HookOn=oldhook.HookOn")
#pragma comment(linker, "/export:HookProc=oldhook.HookProc")

#include <d3d9helper.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

#include "framework.h"

#endif //PCH_H
