#include "pch.h"
#include "main.h"
#include <string>
#include "globals.h"
#include "Hooks/Hooks.h"
#include "Game/memory.h"
#include "Game/player.h"
#include "Game/players.h"
#include "UI/sprite.h"
#include "UI/screen.h"
#include "UI/height.h"
#include "UI/drawfont.h"
#include "UI/scores.h"
#include "UI/drawstats.h"
#include "Mods/battletext.h"
#include "Mods/stats.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// File directory constants
const std::wstring HOME = L"Outlawed\\";
const std::wstring IMGDIR = HOME + L"Images\\";
const std::wstring IMG_RESULTS = L"wanted.png";

// Hooks
tEndScene			oEndScene			= nullptr;
tResetDevice		oResetDevice		= nullptr;
tCloseScreen		oCloseScreen		= nullptr;
tEndofGame			oEndofGame			= nullptr;
tEndofMPlayer		oEndofMPlayer		= nullptr;
tLoadLobbyTexture	oLoadLobbyTexture	= nullptr;
tLobbyandBattleText	oLobbyandBattleText = nullptr;

// pointer to the dummy device
LPDIRECT3DDEVICE9	dev;

// Multiplayer stats
Players* players_list;


// Menu items
Height				Gfx_Height;
bool				mplayer_active = false;
bool				mplayer_end_of_game = false;
bool				display_height = false;
bool				display_stats = false;


// Battle Stuff
Stats stats;
DrawStats draw_stats;

/* Graphics loading here for screen resizes, etc... */
void LoadGraphics()
{
	Gfx_Height.Init();
	draw_stats.Init();
}

/* Release our loaded graphics */
void ReleaseGraphics()
{
	Gfx_Height.Release();
	draw_stats.ReleaseBuffers();
	draw_stats.Release();
}
// Graphics Hooking 
HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{

	/* Multi-player only  - If single player mode just run along like nobody is here... */
	if (!mplayer_active)
		return oEndScene(pDevice);

	/* If the device address changes release and init to the new address */
	if (dev != pDevice)
	{
		dev = pDevice;
		ReleaseGraphics();
		LoadGraphics();
	}


	/* Enable or Disable height sprite */
	if (GetAsyncKeyState(VK_F11) & 1)
	{
		if (!display_height) {
			display_height = true;
		}
		else {
			display_height = false;
		}
	}

	/* Enable / Disable stats sprite */
	if (GetAsyncKeyState(VK_F12) & 1)
	{
		if (!display_stats) {
			display_stats = true;
		}
		else {
			display_stats = false;
		}
	}

	/* Save a copy of your current stats to file */
	if (GetAsyncKeyState(VK_HOME) & 1)
	{
		std::wstring path = IMGDIR + IMG_RESULTS;
		std::wstring save_dir = HOME + L"\\Saves";

		if (draw_stats.StoreBuffer(pDevice)) {
			draw_stats.SaveStatsToFile(path.c_str(), save_dir.c_str(), pDevice, &stats);
			std::wstring weapon = HOME + L"WEAPON.WAV";
			PlaySound((LPCWSTR)weapon.c_str(), NULL, SND_FILENAME | SND_ASYNC);
		}
	}


	/* Draw items to screen */
	if (display_height)	{
		Gfx_Height.Draw(pDevice);
	}

	if (display_stats) {
		draw_stats.DrawStatsToScreen(&stats, pDevice);
	}

	return oEndScene(pDevice);
}

HRESULT APIENTRY hkResetDevice(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	dev = NULL;
	return oResetDevice(pPresentationParameters);
}

void __cdecl hkCloseScreen()
{

	ReleaseGraphics();
	return oCloseScreen();
}

void __stdcall hkEndofMPlayer(void)
{
	mplayer_active = false;
	ReleaseGraphics();
	delete[] players_list;
	return oEndofMPlayer();
}

void __cdecl hkEndofGame(void)
{
	ReleaseGraphics();
	return oEndofGame();
}

// If the lobby texture is loaded we know Multi-Player is in progress
void __stdcall hkLoadLobbyTexture(void)
{
	mplayer_active = true;

	// Zero the player stats and height
	stats = { 0 };
	// Reset players height
	Player::ResetHeight();
	// Create our lobby list of players
	players_list = new Players[MAX_LOBBY_PLAYERS];
	for (int i = 0; i < MAX_LOBBY_PLAYERS; i++) {
		players_list[i].name = (uintptr_t)Memory::base + 0x1cc8f4 + (i * 0x80);
		std::cout << std::hex << players_list[i].name << std::endl;
	}
	

	return oLoadLobbyTexture();
}

// Snag all the messages
void __cdecl hkLobbyandBattleText(char* message, int param2, BYTE param3)
{
	BattleText::Update(message, &stats, players_list);
	return oLobbyandBattleText(message, param2, param3);
}


DWORD WINAPI Main(HMODULE hModule)
{
	Hook hk;
	void* D3D9Device[119];

	FILE* f;
freopen_s(&f, "debug$", "w", stdout);
std::cout << "Hack Thread Init" << std::endl;

	if (!Memory::base)
		return 0;

	// Set up the hook for EndScene 
	if (GetD3D9Device(D3D9Device, sizeof(D3D9Device))) 
	{
		oEndScene			=	(tEndScene)hk.SetTrampHook((BYTE*)D3D9Device[42], (BYTE*)hkEndScene, 7);
		oResetDevice		=	(tResetDevice)hk.SetTrampHook((BYTE*)D3D9Device[16], (BYTE*)hkResetDevice, 5);
		oCloseScreen		=	(tCloseScreen)hk.SetTrampHook((BYTE*)((uintptr_t)Memory::base + 0x65dd0), (BYTE*)hkCloseScreen, 6);
		oEndofGame			=	(tEndofGame)hk.SetTrampHook((BYTE*)((uintptr_t)Memory::base + 0x29fab), (BYTE*)hkEndofGame, 5);
		oEndofMPlayer		=	(tEndofMPlayer)hk.SetTrampHook((BYTE*)((uintptr_t)Memory::base + 0x626C0), (BYTE*)hkEndofMPlayer, 9);
		oLoadLobbyTexture	=	(tLoadLobbyTexture)hk.SetTrampHook((BYTE*)((uintptr_t)Memory::base + 0x0A250), (BYTE*)hkLoadLobbyTexture, 8);
		oLobbyandBattleText =	(tLobbyandBattleText)hk.SetTrampHook((BYTE*)((uintptr_t)Memory::base + 0x7B290), (BYTE*)hkLobbyandBattleText, 5);
	}
	return 0;
}