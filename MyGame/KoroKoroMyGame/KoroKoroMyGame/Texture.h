#include "Source\DirectX3D\DirectX3D.h"

class Texture
{
public: 
	Texture();
	~Texture();

	void create(D3DXVECTOR3, D3DFORMAT);
	void setRenderTarget(INT);
	LPDIRECT3DTEXTURE9 get();
private:
	D3DXVECTOR3 getSize();

	LPDIRECT3DTEXTURE9 tex;
};