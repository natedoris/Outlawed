#pragma once

class DrawFont
{
public:
	DrawFont();

	void	Draw(const char* text, DWORD format, LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE sprite);
	void	Init(int left, int top, int right, int bottom);
	void	Release();

private:
	RECT			rect;
	ID3DXFont*		pFont;
	int				font;
};