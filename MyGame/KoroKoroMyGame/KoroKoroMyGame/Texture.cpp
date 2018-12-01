#include "Texture.h"

Texture::Texture()
{

}


Texture::~Texture()
{

}

void Texture::create(D3DXVECTOR3 size, D3DFORMAT Fmt)
{
	D3DXCreateTexture(DirectX3D::getDevice(),
		(UINT)size.x,
		(UINT)size.y,
		0,
		D3DUSAGE_RENDERTARGET,
		Fmt,
		D3DPOOL_DEFAULT,
		&tex);
}

void Texture::setRenderTarget(INT targetNo)
{
	IDirect3DSurface9 *pSuf;

	tex->GetSurfaceLevel(0, &pSuf);

	DirectX3D::getDevice()->SetRenderTarget(targetNo, nullptr);
	DirectX3D::getDevice()->SetRenderTarget(targetNo, pSuf);

	pSuf->Release();
}

D3DXVECTOR3 Texture::getSize()
{
	IDirect3DSurface9 *Suf;
	tex->GetSurfaceLevel(0, &Suf);


	D3DSURFACE_DESC SufDesc;
	Suf->GetDesc(&SufDesc);

	D3DXVECTOR3 WorkSize;
	WorkSize.x = (FLOAT)SufDesc.Width;
	WorkSize.y = (FLOAT)SufDesc.Height;
	WorkSize.z = 0.0f;

	Suf->Release();
	return WorkSize;
}

LPDIRECT3DTEXTURE9 Texture::get()
{
	return tex;

}