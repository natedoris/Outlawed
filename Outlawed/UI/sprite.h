#ifndef SPRITEUI_H
#define SPRITEUI_H

class Sprite {
public:
	Sprite();
	Sprite(float x, float y);
	~Sprite();

	bool IsInitialized();
	HRESULT Initialize(int width, int height, LPCWSTR file, LPDIRECT3DDEVICE9 pDevice);
	virtual void Update();
	virtual void Draw();
	virtual void Release();

	LPD3DXSPRITE GetSprite();
	

private:
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;
	D3DXVECTOR3 position;
	D3DCOLOR color;
	bool initialized;
};

#endif // !SPRITEUI_H
