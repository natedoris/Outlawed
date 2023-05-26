#include "../pch.h"
#include "drawfont.h"
#include "screen.h"

DrawFont::DrawFont()
{
	pFont = NULL;
	font = 25;
	rect = { 0,0,640,480 };
}

void DrawFont::Draw(const char* text, DWORD format, LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE sprite)
{
	if (!pFont) {
		if (Screen::Right() > 1400)
		{
			font = 35;
		}
		else
		{
			font = 25;
		}
		D3DXCreateFont(pDevice, font, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Cambria", &pFont);
	}

	SetRect(&rect, rect.left, rect.top, rect.right, rect.bottom);

	pFont->DrawTextA(sprite, text, -1, & rect, format, D3DCOLOR_ARGB(125, 255, 255, 85));
}

void DrawFont::Init(int left, int top, int right, int bottom)
{
	if (Screen::CheckCoords())
	{
		rect.left = left;
		rect.top = top;
		rect.right = right;
		rect.bottom = bottom;
	}
}

void DrawFont::Release()
{
	if (pFont)
	{
		pFont->Release();
		pFont = NULL;
	}
}
