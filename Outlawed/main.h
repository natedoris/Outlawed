#pragma once
#include "pch.h"

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(APIENTRY* tResetDevice)(D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(APIENTRY* tRelease)(void);
typedef void(__cdecl* tCloseScreen)(void);
typedef void(__cdecl* tEndofGame)(void);
typedef void(__stdcall* tEndofMPlayer)(void);
typedef void(__stdcall* tLoadLobbyTexture)(void);
typedef void(__cdecl* tLobbyandBattleText)(char* param_1, int param_2, BYTE param_3);

HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice);
HRESULT APIENTRY hkResetDevice(D3DPRESENT_PARAMETERS* pPresentationParameters);

DWORD WINAPI Main(HMODULE hModule);