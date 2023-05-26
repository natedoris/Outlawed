#include "../pch.h"
#include "drawstats.h"
#include "screen.h"
#include "../Mods/stats.h"
#include "../Game/player.h"

DrawStats::DrawStats()
{
	// Set fonts to null
	font_player_name = NULL;
	font_draw_screen_stats = NULL;
	rect = { 0,0,640,480 };

	// Set textures to null
	store_target_surface = NULL;
	store_z_buffer = NULL;
}

void DrawStats::Draw(LPDIRECT3DDEVICE9 pDevice)
{

}

void DrawStats::Init()
{
	if (Screen::CheckCoords())
	{
		rect.left = Screen::Right() - Screen::Left() - 165;
		rect.top = 300;
		rect.right = Screen::Right();
		rect.bottom = Screen::Bottom();
	}
}

void DrawStats::Release()
{
	if (font_draw_screen_stats)
	{
		font_draw_screen_stats->Release();
		font_draw_screen_stats = NULL;
	}
}

bool DrawStats::StoreBuffer(LPDIRECT3DDEVICE9 device)
{
	ReleaseBuffers();

	HRESULT hr_surface = device->GetRenderTarget(0, &store_target_surface);
	HRESULT hr_z_buffer = device->GetDepthStencilSurface(&store_z_buffer);

	if (FAILED(hr_surface) || FAILED(hr_z_buffer)) {
		ReleaseBuffers();
		return false;
	}
	return true;
}

void DrawStats::ReleaseBuffers()
{
	if (store_target_surface) {
		store_target_surface->Release();
		store_target_surface = NULL;
	}
	if (store_z_buffer) {
		store_z_buffer->Release();
		store_z_buffer = NULL;
	}
	if (font_player_name) {
		font_player_name->Release();
		font_player_name = NULL;
	}
}

bool DrawStats::GetFileDimensions(LPCWSTR filename, D3DXIMAGE_INFO* info)
{
	HRESULT hr = D3DXGetImageInfoFromFile(filename, info);

	if (hr != D3D_OK)
	{
		return false;
	}
	return true;
}

void DrawStats::SaveStatsToFile(LPCWSTR filename, std::wstring file_dir, LPDIRECT3DDEVICE9 device, Stats* stats)
{
	HRESULT hr;
	LPDIRECT3DSURFACE9	surface_temp = NULL;
	LPDIRECT3DTEXTURE9	tex_temp = NULL;
	D3DXIMAGE_INFO info;

	// Store the back buffer and z buffer so we can restore original state later
	ReleaseBuffers();
	device->GetRenderTarget(0, &store_target_surface);
	device->GetDepthStencilSurface(&store_z_buffer);
	
	// Return to the game like nothing happened
	if ((store_target_surface && store_z_buffer) == NULL) {
		return;
	}

	// Store file size for creating surface dimensions
	if (!GetFileDimensions(filename, &info)) {
		return;
	}

	// Create texture from file
	if (!tex_temp) {
		hr = D3DXCreateTextureFromFileEx(device, filename,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET,
			D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
			&tex_temp);

		if (FAILED(hr)) {
			return;
		}
	}

	// Build our surface to draw too
	//hr = device->CreateOffscreenPlainSurface(info.Width, info.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface_temp, NULL);
	//if (FAILED(hr)) {
	//	CleanUpTexture(tex_temp);
	//	return;
	//}

	// Get the first surface of the texture
	hr = tex_temp->GetSurfaceLevel(0, &surface_temp);
	if (FAILED(hr)) {
		CleanUpTexture(tex_temp);
		return;
	}

	// Change the devices rendering target to the texture surface
	hr = device->SetRenderTarget(0, surface_temp);

	if (SUCCEEDED(hr)) {

		// Draw our stuff in here
		DrawPlayerName(Player::Name(), device);
		DrawStatsData(stats, device);

		SYSTEMTIME st;
		GetLocalTime(&st);

		wchar_t tmp_t[512];

		std::wstring image_save_location = file_dir + L"\\%d%d-%d-%d.png";

		std::swprintf(tmp_t, 512, image_save_location.c_str(), st.wHour, st.wMinute, st.wDay, st.wYear);

		// Save file
		D3DXSaveTextureToFile(tmp_t, D3DXIFF_PNG, tex_temp, NULL);
	}

	// Restore the rendering target to the original screen 
	hr = device->SetRenderTarget(0, store_target_surface);
	
	hr = device->SetDepthStencilSurface(store_z_buffer);

	// Remove the texture resources
	surface_temp->Release();
	tex_temp->Release();
}

void DrawStats::CleanUpTexture(LPDIRECT3DTEXTURE9 texture)
{
	if (texture) {
		texture->Release();
		texture = NULL;
	}
}

void DrawStats::DrawPlayerName(std::string player, LPDIRECT3DDEVICE9 device)
{
	if (!font_player_name) {
		// Build our massive player font!
		HRESULT hr = D3DXCreateFont(device, 65, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Cambria", &font_player_name);
	}

	RECT rect{ 210, 200, 810, 260 };

	font_player_name->DrawTextA(NULL, player.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
	font_player_name->Release();
	font_player_name = NULL;
}

void DrawStats::DrawStatsData(Stats* stats, LPDIRECT3DDEVICE9 device)
{
	ID3DXFont* font;
	font = NULL;

	HRESULT hr = D3DXCreateFont(device, 35, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Cambria", &font);
	
	RECT rect{ 210, 275, 810, 600 };

	wchar_t buf[512];
	std::swprintf(buf, 512, 
		L"Kills: %d\tDeaths: %d\n"
		L"TNT-K: %d\tTNT-D: %d\n"
		L"Fell : %d\n"
		L"Crushed : %d\n"
		L"Drowned : %d\n"
		L"KDR : %0.2f\n"
		L"Max Height Flown : %0.2f",
		stats->kills, stats->deaths, 
		stats->kills_from_dynamite, 
		stats->death_from_dynamite, 
		stats->suicides_fell, 
		stats->suicides_crushed,
		stats->suicides_drowned,
		stats->kdr,
		Player::MaxHeight());

	font->DrawTextW(NULL, buf, -1, &rect, DT_CENTER | DT_NOCLIP | DT_EXPANDTABS, D3DCOLOR_ARGB(255, 0, 0, 0));
	font->Release();
	font = NULL;
}

void DrawStats::DrawStatsToScreen(Stats* stats, LPDIRECT3DDEVICE9 device)
{
	if (!font_draw_screen_stats)
	{
		HRESULT hr = D3DXCreateFont(device, 20, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Cambria", &font_draw_screen_stats);
	}

	SetRect(&rect, rect.left, rect.top, rect.right, rect.bottom);

	wchar_t buf[512];
	std::swprintf(buf, 512,
		L"Kills: %d\tDeaths: %d\n"
		L"TNT-K: %d\tTNT-D: %d\n"
		L"Fell : %d\n"
		L"KDR : %0.2f\n",
		stats->kills, stats->deaths,
		stats->kills_from_dynamite,
		stats->death_from_dynamite,
		stats->suicides_fell,
		stats->kdr);

	font_draw_screen_stats->DrawTextW(NULL, buf, -1, &rect, DT_LEFT | DT_NOCLIP | DT_EXPANDTABS, D3DCOLOR_ARGB(255, 255, 255, 255));
}
