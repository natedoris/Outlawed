#pragma once
#include "IDrawFont.h"
#include "../Mods/stats.h"

class DrawStats : public IDrawFont 
{
public:
	DrawStats();
	void Draw(LPDIRECT3DDEVICE9 pDevice);
	void Init();
	void Release();

	void SaveStatsToFile(LPCWSTR filename, std::wstring file_dir, LPDIRECT3DDEVICE9 device, Stats* stats);
	bool			StoreBuffer(LPDIRECT3DDEVICE9 device);
	void			ReleaseBuffers();
	void			DrawStatsToScreen(Stats* stats, LPDIRECT3DDEVICE9 device);

private:
	ID3DXFont*			font_player_name;
	ID3DXFont*			font_draw_screen_stats;
	RECT				rect;
	LPDIRECT3DSURFACE9	store_target_surface;
	LPDIRECT3DSURFACE9	store_z_buffer;



	bool			GetFileDimensions(LPCWSTR filename, D3DXIMAGE_INFO* info);
	void			CleanUpTexture(LPDIRECT3DTEXTURE9 texture);
	void			DrawStatsData(Stats* stats, LPDIRECT3DDEVICE9 device);
	void			DrawPlayerName(std::string player, LPDIRECT3DDEVICE9 device);
};