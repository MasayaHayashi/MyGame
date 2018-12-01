#ifndef POST_SHADER_H
#define POST_SHADER_H


#include "Source\DirectX3D\DirectX3D.h"

class SpriteRenderer final
{
public :
	SpriteRenderer();
	~SpriteRenderer();

	static void create();
	static void draw();

private:
	static LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// 頂点バッファインターフェースへのポインタ
};

#endif