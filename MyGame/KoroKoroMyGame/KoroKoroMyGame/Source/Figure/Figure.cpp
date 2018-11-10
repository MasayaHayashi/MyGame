//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Figure.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Figure.h"
#include "../Mesh/Mesh.h"
#include "../DirectX3D/DirectX3D.h"
#include "../KeyBoard/Keyboard.h"
#include "../SceneManager/SceneManager.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Figure::Figure()
{
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxTranslate);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxScale);

	Translate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Rot		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scale	  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	cubeTexture  = nullptr;
	cubeVtx		 = nullptr;
	pD3DXBuffMat = nullptr;
	pD3DXMesh	 = nullptr;
	lNumMat		 = 0;

	Material.MatD3D.Ambient.r = 1.0f;
	Material.MatD3D.Ambient.g = 0.0f;
	Material.MatD3D.Ambient.b = 0.0f;
	Material.MatD3D.Ambient.a = 0.0f;

	Material.MatD3D.Diffuse.r = 1.0f;
	Material.MatD3D.Diffuse.g = 0.0f;
	Material.MatD3D.Diffuse.b = 0.0f;
	Material.MatD3D.Diffuse.a = 0.0f;

	Material.MatD3D.Emissive.r = 0.0f;
	Material.MatD3D.Emissive.g = 1.0f;
	Material.MatD3D.Emissive.b = 0.0f;
	Material.MatD3D.Emissive.a = 0.0f;

	Material.MatD3D.Power = 1.0f;

	Material.MatD3D.Specular.r = 0.0f;
	Material.MatD3D.Specular.g = 1.0f;
	Material.MatD3D.Specular.b = 1.0f;
	Material.MatD3D.Specular.a = 0.0f;

	isUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ (引数: Pawn)
// 位置、サイズ指定
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Figure::Figure(D3DXVECTOR3 SetPos, D3DXVECTOR3 SetSize)
{
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxTranslate);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxScale);

	Translate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Rot		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scale	  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	cubeVtx		= nullptr;
	cubeTexture = nullptr;
	pD3DXBuffMat = nullptr;
	pD3DXMesh	 = nullptr;
	lNumMat		 = 0;

	Material.MatD3D.Ambient.r = 1.0f;
	Material.MatD3D.Ambient.g = 0.0f;
	Material.MatD3D.Ambient.b = 0.0f;
	Material.MatD3D.Ambient.a = 0.0f;

	Material.MatD3D.Diffuse.r = 1.0f;
	Material.MatD3D.Diffuse.g = 0.0f;
	Material.MatD3D.Diffuse.b = 0.0f;
	Material.MatD3D.Diffuse.a = 0.0f;

	Material.MatD3D.Emissive.r = 0.0f;
	Material.MatD3D.Emissive.g = 1.0f;
	Material.MatD3D.Emissive.b = 0.0f;
	Material.MatD3D.Emissive.a = 0.0f;

	Material.MatD3D.Power = 1.0f;

	Material.MatD3D.Specular.r = 0.0f;
	Material.MatD3D.Specular.g = 1.0f;
	Material.MatD3D.Specular.b = 1.0f;
	Material.MatD3D.Specular.a = 0.0f;

	pBBox = nullptr;
	isUsed = true;

	// 位置、サイズセット
	Pos	  = SetPos;
	Size  = SetSize;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	color = D3DXCOLOR(DeaultBoxcolor);

	D3DXCreateBox(devicePtr, Size.x * 2.0f, Size.y * 2.0f, Size.z * 2.0f, &pBBox, nullptr);
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Figure::~Figure()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::initializeFigure()
{
	D3DXMatrixTranslation(&mtxWorld, Pos.x, Pos.y * 2, Pos.z);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::initializeFigure(D3DXVECTOR3 SetPos, D3DXVECTOR3 SetSize,D3DXVECTOR3 SetCenterPos)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	Pos		  = SetPos;
	Size	  = SetSize;
	CenterPos = SetCenterPos;

	color = D3DXCOLOR(DeaultBoxcolor);

	D3DXCreateBox(devicePtr, Size.x * 2.0f, Size.y * 2.0f, Size.z * 2.0f, &pBBox, nullptr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::UpdateFigure(D3DXMATRIX SetMtx, D3DXCOLOR Setcolor)
{
	D3DXMATRIX trans;

	mtxWorld = SetMtx;
	D3DXMatrixTranslation(&trans, 0.0f, Size.y, 0.0f);
	mtxWorld *= trans;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::DrawFigure()
{
	// 例外処理
	if (!isUsed)
		return;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	
	devicePtr->SetTransform(D3DTS_WORLD, &mtxWorld);

	devicePtr->SetTexture(0, nullptr);

	ZeroMemory(&Material, sizeof(Material));

	Material.MatD3D.Diffuse = color;
	devicePtr->SetMaterial(&Material.MatD3D);

	pBBox->DrawSubset(0);
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::UninitFigure()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 立方体頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Figure::MakeVtxCube(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	nNumVertex = 36;// * 2;	// 頂点数

	// オブジェクトの頂点バッファを生成
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex ,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&cubeVtx,					// 頂点バッファインターフェースへのポインタ
		nullptr)))						// nullptrに設定
	{
		return E_FAIL;
	}

	{
		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		cubeVtx->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		
		// 前方向
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);

		pVtx[3].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[4].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		pVtx[5].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);

		// 右方向
		pVtx[6].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[7].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[8].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		pVtx[9].vtx  = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[10].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[11].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		// 下方向
		pVtx[12].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		pVtx[13].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[14].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		pVtx[15].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[16].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[17].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		// 左方向
		pVtx[18].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[19].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[20].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		pVtx[21].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[22].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[23].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		// 奥方向
		pVtx[24].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[25].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[26].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);

		pVtx[27].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[28].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[29].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		
		// 上方向
		pVtx[30].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[31].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[32].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		
		pVtx[33].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[34].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[35].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		
		pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[8].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[9].nor = D3DXVECTOR3( 1.0f,  0.0f, 0.0f);
		pVtx[10].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[11].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		
		pVtx[12].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[13].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[14].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[15].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[16].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[17].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		
		pVtx[18].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[19].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		
		pVtx[24].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[25].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[26].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[27].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[28].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[29].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		
		pVtx[30].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[31].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[32].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[33].nor = D3DXVECTOR3(0.0f, -1.0f, 0.3f);
		pVtx[34].nor = D3DXVECTOR3(0.0f, -1.0f, 0.3f);
		pVtx[35].nor = D3DXVECTOR3(0.0f, -1.0f, 0.3f);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);

		pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[4].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[5].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.6f, 0.8f);
		
		pVtx[6].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.8f);
		pVtx[7].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
		pVtx[8].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);

		pVtx[9].diffuse  = D3DXCOLOR(0.0f, 0.6f, 1.0f, 0.8f);
		pVtx[10].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
		pVtx[11].diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[12].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[13].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		pVtx[15].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[16].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[18].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[19].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[21].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[22].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[24].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[25].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[26].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		pVtx[27].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.8f);
		pVtx[28].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[29].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[30].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[31].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[32].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		pVtx[33].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.8f);
		pVtx[34].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[35].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
	}
}

void Figure::CreateTexture()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Figure::GetPos()
{
	return D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::SetPos(D3DXVECTOR3 Pos)
{
	mtxWorld._41 = Pos.x;
	mtxWorld._42 = Pos.y;
	mtxWorld._43 = Pos.z;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// サイズ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Figure::GetSize()
{
	return Size;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用フラグセット用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::setUsedFlg(bool bSet)
{
	isUsed = bSet;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// マテリアル色セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Figure::SetMatcolor(D3DCOLOR Setcolor)
{
	color = Setcolor;
}