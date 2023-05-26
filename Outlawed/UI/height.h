#pragma once
#include "../pch.h"
#include "IDrawFont.h"

class Height : public IDrawFont {
public:
	Height();
	void			Draw(LPDIRECT3DDEVICE9 pDevice);
	void			Init();
	void			Release();

private:
	RECT			rect;
	ID3DXFont*		pFont;
	int				font;
};