#include "PostSpriteRenderer.h"
#include "shader.h"

LPDIRECT3DVERTEXBUFFER9 SpriteRenderer::pD3DVtxBuff;		// 頂点バッファインターフェースへのポインタ

SpriteRenderer::SpriteRenderer()
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (Vertex),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_2D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		nullptr)))										// nullptrに設定

}
SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::draw()
{
	// 頂点バッファの中身を埋める
	VERTEX_2D  *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(右回り)
	D3DXVECTOR3 Pos;
	Pos = pVtx[0].vtx;
	pVtx[0].vtx.x = -1.0f;
	pVtx[0].vtx.y = 1.0f;
	pVtx[0].vtx.z = 0.01f;

	Pos = pVtx[1].vtx;
	pVtx[1].vtx.x = 1.0f;
	pVtx[1].vtx.y = 1.0f;
	pVtx[1].vtx.z = 0.01f;

	Pos = pVtx[2].vtx;
	pVtx[2].vtx.x = -1.0f;
	pVtx[2].vtx.y = -1.0f;
	pVtx[2].vtx.z = 0.01f;

	Pos = pVtx[3].vtx;
	pVtx[3].vtx.x = 1.0f;
	pVtx[3].vtx.y = -1.0f;
	pVtx[3].vtx.z = 0.01f;


	// 頂点カラーの設定(0～255の間)
	D3DXCOLOR Col(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].diffuse = Col;
	pVtx[1].diffuse = Col;
	pVtx[2].diffuse = Col;
	pVtx[3].diffuse = Col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	pD3DVtxBuff->Unlock();

	auto pDevice = DirectX3D::getDevice();
	pDevice->SetStreamSource(0, pD3DVtxBuff, 0, sizeof(VERTEX_2D));

#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	// テクスチャの設定
	Shader::RenderStart();


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	Shader::RenderEnd();
}