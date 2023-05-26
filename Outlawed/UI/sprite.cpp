#include "../pch.h"
#include "sprite.h"

Sprite::Sprite()
{
	tex = NULL;
	sprite = NULL;
	position.x = 0;
	position.y = 0;
	position.z = 0;

	color = D3DCOLOR_ARGB(255, 255, 255, 255);
	initialized = false;
}

Sprite::Sprite(float x, float y)
{
	tex = NULL;
	sprite = NULL;
	position.x = x;
	position.y = y;
	position.z = 0;

	color = D3DCOLOR_ARGB(255, 255, 255, 255);
	initialized = false;
}

Sprite::~Sprite()
{


	if (tex != NULL) 
	{
		tex->Release();
		tex = 0;
	}
	if (sprite != NULL)
	{
		sprite->Release();
		sprite = 0;
	}
	initialized = false;
}

bool Sprite::IsInitialized()
{
	return initialized;
}

HRESULT Sprite::Initialize(int width, int height, LPCWSTR file, LPDIRECT3DDEVICE9 pDevice)
{
	// Create the texture
	if (FAILED(D3DXCreateTextureFromFileEx(pDevice, file, width, height, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tex)))
	{
		MessageBox(0, L"FAILED to create texture", NULL, MB_OK);
		return E_FAIL;
	}

	// Create the sprite
	if (FAILED(D3DXCreateSprite(pDevice, &sprite)))
	{
		MessageBox(0, L"FAILED to create sprite", NULL, MB_OK);
		return E_FAIL;
	}
	initialized = true;
	return S_OK;
}

void Sprite::Update()
{
}

void Sprite::Draw()
{
	if (tex && sprite)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw(tex, NULL, NULL, &position, color);
		sprite->End();
	}
}

void Sprite::Release()
{
	if (tex != NULL)
	{
		tex->Release();
		tex = 0;
	}
	if (sprite != NULL)
	{
		sprite->Release();
		sprite = 0;
	}

	initialized = false;
}

LPD3DXSPRITE Sprite::GetSprite()
{
	return sprite;
}