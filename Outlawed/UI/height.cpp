#include "../pch.h"
#include "height.h"
#include "screen.h"
#include "../Game/player.h"
#include "../Hooks/gh_d3d9.h"


Height::Height()
{
	pFont = NULL;
	font = 25;
	rect = { 0,0,640,480 };
}

void Height::Draw(LPDIRECT3DDEVICE9 pDevice)
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

	char		cHeightBuf[64];
	char		szHeightBuf[256] = "Height\n";
	char		cMaxHeightBuf[64];
	char		szMaxHeightBuf[256] = "Max Height\n";
	char		szFinalBuf[512];

	int ret = snprintf(cHeightBuf, sizeof(cHeightBuf), "%0.0f ft\n", Player::CurrentHeight());
	strcat_s(szHeightBuf, sizeof(szHeightBuf), cHeightBuf);
	snprintf(cMaxHeightBuf, sizeof(cMaxHeightBuf), "%0.2f ft", Player::MaxHeight());
	strcat_s(szMaxHeightBuf, sizeof(szMaxHeightBuf), cMaxHeightBuf);
	strcpy_s(szFinalBuf, sizeof(szFinalBuf), szHeightBuf);
	strcat_s(szFinalBuf, sizeof(szFinalBuf), szMaxHeightBuf);

	pFont->DrawTextA(NULL, szFinalBuf, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void Height::Init()
{
	if (Screen::CheckCoords())
	{
		rect.left = Screen::Left() + 15;
		rect.top = 300;
	}
}

void Height::Release()
{
	if (pFont) {
		pFont->Release();
		pFont = NULL;
	}
}

