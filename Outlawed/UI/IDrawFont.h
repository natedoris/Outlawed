#pragma once
#include "../pch.h"

class IDrawFont 
{
public:
	virtual void	Draw(LPDIRECT3DDEVICE9 pDevice);
	virtual void	Init();
	virtual void	Release();
};