//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_ResourceManager.cpp
// リソース管理(メッシュ、テクスチャ管理)
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

#define _CRT_SECURE_NO_WARNINGS

// ===== インクルード部 =====
#include "C_ResourceManager.h"
#include <string>
#include <tchar.h>			// 汎用テキスト マッピング
#include <utility>
#include "C_3DPawn.h"
#include "C_Board.h"

// ===== 名前空間定義 =====
using namespace std;

// ===== 静的メンバ変数 =====
C_RESOURCE_MANAGER*					C_RESOURCE_MANAGER::pInstance;				// インスタンス
std::vector<MESH_DATA*>				C_RESOURCE_MANAGER::Mesh;					// メッシュ情報
std::vector<HIERARCHY_MESH_DATA*>	C_RESOURCE_MANAGER::HierarchyMesh;			// 階層構造用メッシュ情報
std::vector<TEXTURE_DATA*>			C_RESOURCE_MANAGER::Texture;				// テクスチャ情報
std::vector<TEXTURE_DATA*>			C_RESOURCE_MANAGER::FadeTexture;			// テクスチャ情報
std::vector<VERTEX_BOARD_DATA*>		C_RESOURCE_MANAGER::VtxBoard;				// ボード頂点情報
std::vector<VERTEX_BOARD_DATA*>		C_RESOURCE_MANAGER::VtxFadeBoard;			// ボード頂点情報

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_RESOURCE_MANAGER::C_RESOURCE_MANAGER()
{
	pInstance = NEW C_RESOURCE_MANAGER;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_RESOURCE_MANAGER::~C_RESOURCE_MANAGER()
{
	SAFE_DELETE(pInstance);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデル生成(階層構造無し)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_RESOURCE_MANAGER::MakeModel(MESH_DATA &MeshData, CHAR *pszFilename, UINT &uMeshType)
{
	// 例外処理
	if (!pszFilename)
		return E_FAIL;

	// メッシュの種類
	uMeshType = (UINT)NORMAL_MODEL;

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// 既に生成されているか
	if (CheckExisting(pszFilename, MeshData))
		return S_OK;

	// ファイル名セット
	strcpy(MeshData.szMeshFileName, pszFilename);

	// メッシュ情報セット
	Mesh.push_back(&MeshData);
	
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(pszFilename, D3DXMESH_SYSTEMMEM, devicePtr, NULL, &Mesh.back()->pD3DXBuffMat, NULL, &Mesh.back()->uNumMat, &Mesh.back()->pD3DXMesh)))
	{
		return E_FAIL;
	}

	// 属性取得のための下準備
	LPD3DXMESH pMeshWk;
	HRESULT hr = Mesh.back()->pD3DXMesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pMeshWk);

	if (SUCCEEDED(hr))
	{
		Mesh.back()->pD3DXMesh->Release();
		Mesh.back()->pD3DXMesh = pMeshWk;
	}
	else
	{
		SAFE_RELEASE(Mesh.back()->pD3DXBuffMat);	// マテリアル解放
		return false;
	}

	// 属性テーブル取得
	Mesh.back()->dwAttrNum = 0;
	Mesh.back()->pD3DXMesh->GetAttributeTable(NULL, &Mesh.back()->dwAttrNum);
	Mesh.back()->pAttr = NEW D3DXATTRIBUTERANGE[Mesh.back()->dwAttrNum];
	Mesh.back()->pD3DXMesh->GetAttributeTable(Mesh.back()->pAttr, &Mesh.back()->dwAttrNum);

	// 指定の頂点フォーマットに変換
	DWORD dwFVF = Mesh.back()->pD3DXMesh->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pMeshTmp = Mesh.back()->pD3DXMesh;
		pMeshTmp->CloneMeshFVF(pMeshTmp->GetOptions(), FVF_TVERTEX, devicePtr, &Mesh.back()->pD3DXMesh);
		SAFE_RELEASE(pMeshTmp);
		// 法線が無い場合は強制的に追加
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(Mesh.back()->pD3DXMesh, NULL);
		}
	}

	// 頂点情報サイズ数取得
	Mesh.back()->dwNumVtx = Mesh.back()->pD3DXMesh->GetNumVertices();	// 頂点数取得
	Mesh.back()->pVtx = NEW MESH_VTX[Mesh.back()->dwNumVtx];
	LPVOID pWork;

	// 頂点バッファアンロック
	Mesh.back()->pD3DXMesh->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(Mesh.back()->pVtx, pWork, sizeof(MESH_VTX) * Mesh.back()->dwNumVtx);

	// 頂点バッファロック
	Mesh.back()->pD3DXMesh->UnlockVertexBuffer();

	// インデックスバッファから読み込み
	Mesh.back()->dwNumIndx = Mesh.back()->pD3DXMesh->GetNumFaces() * 3;
	Mesh.back()->pIndx = NEW WORD[Mesh.back()->dwNumIndx];

	// インデックスバッファロック
	Mesh.back()->pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(Mesh.back()->pIndx, pWork, sizeof(WORD) * Mesh.back()->dwNumIndx);

	// インデックスバッファアンロック
	Mesh.back()->pD3DXMesh->UnlockIndexBuffer();

	// 境界ボックスと中心座標を求める
	Mesh.back()->MaxVtx = Mesh.back()->pVtx[0].VtxPos;
	Mesh.back()->MinVtx = Mesh.back()->pVtx[0].VtxPos;
	for (WORD i = 1; i < Mesh.back()->dwNumVtx; i++)
	{
		// 最大値取得
		if (Mesh.back()->MaxVtx.x < Mesh.back()->pVtx[i].VtxPos.x)
			Mesh.back()->MaxVtx.x = Mesh.back()->pVtx[i].VtxPos.x;

		if (Mesh.back()->MaxVtx.y < Mesh.back()->pVtx[i].VtxPos.y)
			Mesh.back()->MaxVtx.y = Mesh.back()->pVtx[i].VtxPos.y;

		if (Mesh.back()->MaxVtx.z < Mesh.back()->pVtx[i].VtxPos.z)
			Mesh.back()->MaxVtx.z = Mesh.back()->pVtx[i].VtxPos.z;

		// 最小値取得
		if (Mesh.back()->MinVtx.x > Mesh.back()->pVtx[i].VtxPos.x)
			Mesh.back()->MinVtx.x = Mesh.back()->pVtx[i].VtxPos.x;

		if (Mesh.back()->MinVtx.y > Mesh.back()->pVtx[i].VtxPos.y)
			Mesh.back()->MinVtx.y = Mesh.back()->pVtx[i].VtxPos.y;

		if (Mesh.back()->MinVtx.z > Mesh.back()->pVtx[i].VtxPos.z)
			Mesh.back()->MinVtx.z = Mesh.back()->pVtx[i].VtxPos.z;
	}

	// 中心座標、あたり判定取得
	Mesh.back()->CenterPos = (Mesh.back()->MaxVtx + Mesh.back()->MinVtx) * 0.5f;
	Mesh.back()->CollitionBox = (Mesh.back()->MaxVtx - Mesh.back()->MinVtx) * 0.5f;

	// 境界球取得
	FLOAT dx = Mesh.back()->pVtx[0].VtxPos.x - Mesh.back()->CenterPos.x;
	FLOAT dy = Mesh.back()->pVtx[0].VtxPos.y - Mesh.back()->CenterPos.y;
	FLOAT dz = Mesh.back()->pVtx[0].VtxPos.z - Mesh.back()->CenterPos.z;
	Mesh.back()->fCollisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < Mesh.back()->dwNumVtx; i++)
	{
		dx = Mesh.back()->pVtx[i].VtxPos.x - Mesh.back()->CenterPos.x;
		dy = Mesh.back()->pVtx[i].VtxPos.y - Mesh.back()->CenterPos.y;
		dz = Mesh.back()->pVtx[i].VtxPos.z - Mesh.back()->CenterPos.z;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (Mesh.back()->fCollisionRadus < fRadius)
			Mesh.back()->fCollisionRadus = fRadius;
	}

	// 中心座標をワールド座標系に変換
	//Mesh[pszFilename].CenterPos += PosPawn;

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_RESOURCE_MANAGER::MakeVertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszName)
{
	// 識別用ファイル名セット
	strcpy(VtxBordData.szName, pszName);

	// フェード用判定
	if (VtxBordData.bFade)
		MakeVtxFade(VtxBordData);
	else
		MakeVtx(VtxBordData);

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::MakeVtx(VERTEX_BOARD_DATA &VtxBoardData)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// 頂点情報情報セット
	VtxBoard.push_back(&VtxBoardData);

	// 頂点情報生成
	switch (VtxBoardData.BoardType)
	{
	case POLYGON_2D:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX),	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&VtxBoard.back()->pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))										// NULLに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			VtxBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y + VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y + VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y - VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y - VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);

				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// 頂点データをアンロックする
			VtxBoard.back()->pD3DVtxBuffBoard->Unlock();
		}
		break;
	case BILLBOARD:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_3D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&VtxBoard.back()->pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))										// NULLに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// 法線の設定
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// 頂点データをアンロックする
		VtxBoard.back()->pD3DVtxBuffBoard->Unlock();
	}
	break;
	case POLYGON_3D:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&VtxBoardData.pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// 法線の設定
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// 頂点データをアンロックする
		VtxBoard.back()->pD3DVtxBuffBoard->Unlock();

		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード用頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::MakeVtxFade(VERTEX_BOARD_DATA &VtxBoardData)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// 頂点情報情報セット
	VtxFadeBoard.push_back(&VtxBoardData);

	// 頂点情報生成
	switch (VtxBoardData.BoardType)
	{
	case POLYGON_2D:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX),	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&VtxFadeBoard.back()->pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))										// NULLに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			VtxFadeBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y + VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y + VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y - VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y - VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);

				// rhwの設定
				pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// 頂点データをアンロックする
			VtxFadeBoard.back()->pD3DVtxBuffBoard->Unlock();
		}
		break;
	case BILLBOARD:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_3D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&VtxFadeBoard.back()->pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))										// NULLに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// 法線の設定
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// 頂点データをアンロックする
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Unlock();
	}
	break;
	case POLYGON_3D:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&VtxFadeBoard.back()->pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// 法線の設定
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// 頂点データをアンロックする
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Unlock();

		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_RESOURCE_MANAGER::CreateTexture(TEXTURE_DATA &TextureData,  CHAR *pszFilename)
{
	// 例外処理
	if (!pszFilename)
		return E_FAIL;

	// 既に生成されているか
	if (CheckExisting(pszFilename, TextureData))
		return S_OK;

	// ファイル名セット
	strcpy(TextureData.szTexFileName, pszFilename);

	Texture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, Texture.back()->szTexFileName, &Texture.back()->pD3DTexture))
		return S_OK;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_RESOURCE_MANAGER::MakeModelHierarchy(HIERARCHY_MESH_DATA &HierarchyMedhData,CHAR *pszFilename, UINT &uMeshType)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// メッシュの種類初期化
	uMeshType = (INT)HIERARCHY_MODEL;

	// メッシュ情報セット
	strcpy(HierarchyMedhData.szMeshFileName, pszFilename);	// ファイル名セット

	// メッシュ情報セット
	HierarchyMesh.push_back(&HierarchyMedhData);

	// ディレクトリ抽出
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(HierarchyMesh.back()->szMeshFileName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	HierarchyMesh.back()->Hierarchy.SetDirectory(szDir);

	// 階層構造メッシュの読み込み
	HRESULT hr = D3DXLoadMeshHierarchyFromX(HierarchyMesh.back()->szMeshFileName, D3DXMESH_MANAGED, devicePtr, &HierarchyMesh.back()->Hierarchy, NULL, &HierarchyMesh.back()->pFrameRoot, &HierarchyMesh.back()->pAnimCtrl);
	if (FAILED(hr))
		return false;

	// ボーンとフレームの関連付け
	hr = AllocAllBoneMatrix(HierarchyMesh.back()->pFrameRoot,pszFilename);
	if (FAILED(hr)) return false;

	// アニメーションセット取得
	HierarchyMesh.back()->uNumAnimSet = 0;
	if (HierarchyMesh.back()->pAnimCtrl)
	{
		HierarchyMesh.back()->uNumAnimSet = HierarchyMesh.back()->pAnimCtrl->GetNumAnimationSets();
		if (HierarchyMesh.back()->uNumAnimSet > 0)
		{
			HierarchyMesh.back()->ppAnimSet = NEW LPD3DXANIMATIONSET[HierarchyMesh.back()->uNumAnimSet];
			for (DWORD u = 0; u < HierarchyMesh.back()->uNumAnimSet; u++)
			{
				HierarchyMesh.back()->pAnimCtrl->GetAnimationSet(u, &HierarchyMesh.back()->ppAnimSet[u]);
			}
		}
	}

	if (HierarchyMesh.back()->pFrameRoot)
	{
		// マトリックス更新
		SetTime(0.0,pszFilename);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		UpdateFrameMatrices(HierarchyMesh.back()->pFrameRoot, &world);

		// 境界球/境界ボックス取得
		CalcCollision(HierarchyMesh.back()->pFrameRoot,pszFilename);
	}

	// 経過時間計測用時刻設定
	HierarchyMesh.back()->dwPrev = timeGetTime();

	return SUCCEEDED(hr);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレームのマトリックスを更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_RESOURCE_MANAGER::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	// 親のマトリックスを掛け合わせる
	if (pParentMatrix)
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	// 兄弟フレームがあればマトリックスを更新
	if (pFrame->pFrameSibling)
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);

	// 子フレームがあればマトリックスを更新
	if (pFrame->pFrameFirstChild)
	{
		// 自分が親となるので、自分のマトリックスを渡す
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメーション開始時間設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_RESOURCE_MANAGER::SetTime(DOUBLE dTime,CHAR *pszFilename)
{
	if (HierarchyMesh.back()->pAnimCtrl == NULL)
		return;

	for (DWORD i = 0; i < HierarchyMesh.back()->pAnimCtrl->GetMaxNumTracks(); ++i)
	{
		HierarchyMesh.back()->pAnimCtrl->SetTrackPosition(i, 0);
	}

	HierarchyMesh.back()->pAnimCtrl->ResetTime();
	HierarchyMesh.back()->pAnimCtrl->AdvanceTime(dTime, NULL);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_RESOURCE_MANAGER::AllocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrBase;

	if (meshContainerPtr->pSkinInfo == NULL)
		return S_OK;	// スキン情報が無ければ何もしない

	DWORD dwBoneNum = meshContainerPtr->pSkinInfo->GetNumBones();
	meshContainerPtr->ppBoneMatrix = NEW LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(HierarchyMesh.back()->pFrameRoot, meshContainerPtr->pSkinInfo->GetBoneName(i));

		if (pFrame == NULL)
			return E_FAIL;
		meshContainerPtr->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_RESOURCE_MANAGER::AllocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->meshContainerPtr)
	{
		hr = AllocBoneMatrix(pFrame->meshContainerPtr,pszFilename);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameSibling)
	{
		hr = AllocAllBoneMatrix(pFrame->pFrameSibling,pszFilename);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameFirstChild)
		hr = AllocAllBoneMatrix(pFrame->pFrameFirstChild,pszFilename);

	return hr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//  階層構造用 境界球/境界ボックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_RESOURCE_MANAGER::CalcCollision(LPD3DXFRAME pFrame,CHAR *pszFilename)
{
	D3DXVECTOR3& vMax = HierarchyMesh.back()->CollitionBox;
	D3DXVECTOR3& vMin = HierarchyMesh.back()->CenterPos;

	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	
	HierarchyMesh.back()->fCollisionRadus = -1.0f;
	CalcCollisionFrame(pFrame,pszFilename);
	
	D3DXVECTOR3 vBBox, vCenter;
	vBBox = (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	
	HierarchyMesh.back()->CollitionBox = vBBox;
	HierarchyMesh.back()->CenterPos = vCenter;
	HierarchyMesh.back()->fCollisionRadus = 0.0f;
	CalcCollisionFrame(pFrame,pszFilename);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレーム毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_RESOURCE_MANAGER::CalcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	LPD3DXMESHCONTAINER meshContainerPtr = pFrame->meshContainerPtr;
	while (meshContainerPtr)
	{
		// メッシュコンテナ毎の頂点座標の抽出
		CalcCollisionMeshContainer(meshContainerPtr, pFrame,pszFilename);
		// 次のメッシュコンテナ
		meshContainerPtr = meshContainerPtr->pNextMeshContainer;
	}
	// 兄弟フレームがあれば兄弟フレームを描画
	if (pFrame->pFrameSibling)
		CalcCollisionFrame(pFrame->pFrameSibling,pszFilename);
	// 子フレームがあれば子フレームを描画
	if (pFrame->pFrameFirstChild)
		CalcCollisionFrame(pFrame->pFrameFirstChild,pszFilename);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュコンテナ毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_RESOURCE_MANAGER::CalcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->CombinedTransformationMatrix;
	// 頂点座標の抽出
	LPD3DXMESH pMesh = meshContainerPtr->MeshData.pMesh;
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVtx = pMesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (HierarchyMesh.back()->fCollisionRadus < 0.0f)
		{
			D3DXVECTOR3& vMax = HierarchyMesh.back()->CollitionBox;
			D3DXVECTOR3& vMin = HierarchyMesh.back()->CenterPos;
			if (vMax.x < vtx.x) vMax.x = vtx.x;
			if (vMax.y < vtx.y) vMax.y = vtx.y;
			if (vMax.z < vtx.z) vMax.z = vtx.z;
			if (vMin.x > vtx.x) vMin.x = vtx.x;
			if (vMin.y > vtx.y) vMin.y = vtx.y;
			if (vMin.z > vtx.z) vMin.z = vtx.z;
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - HierarchyMesh.back()->CenterPos));
			if (HierarchyMesh.back()->fCollisionRadus < fRadius)
				HierarchyMesh.back()->fCollisionRadus = fRadius;
		}
	}
	pMesh->UnlockVertexBuffer();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::CheckExisting(CHAR *pszChakNeme,MESH_DATA &MeshData)
{
	// 全メッシュを検索
	for (UINT i = 0; i < Mesh.size(); i++)
		if (strcmp(Mesh[i]->szMeshFileName, pszChakNeme) == 0)
		{
			MeshData = *Mesh[i];	// 生成済みなら元のデータを使用
			return true;
		}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::CheckExisting(CHAR *pszChakNeme, TEXTURE_DATA &TextureData)
{
	// 全テクスチャを検索
	for (UINT i = 0; i < Texture.size(); i++)
		if (strcmp(Texture[i]->szTexFileName, pszChakNeme) == 0)
		{
			TextureData = *Texture[i];	// 生成済みなら元のデータを使用
			return true;
		}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::CheckExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &TextureData)
{
	// 全テクスチャを検索
	for (UINT i = 0; i < VtxBoard.size(); i++)
		if (strcmp(VtxBoard[i]->szName, pszChakNeme) == 0)
		{
			TextureData = *VtxBoard[i];	// 生成済みなら元のデータを使用
			return true;
		}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::DestroyMesh(CHAR *pszChakNeme)
{
	for (INT i = (INT)Mesh.size() - 1; i >= 0; i--)
		if (strcmp(Mesh[i]->szMeshFileName, pszChakNeme) == 0)
		{
			SAFE_DELETE(Mesh[i]->pAttr);
			SAFE_DELETE(Mesh[i]->pIndx);
			SAFE_DELETE(Mesh[i]->pVtx);
			Mesh.erase(Mesh.begin() + i);
			return true;
		}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 全メッシュ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::DestroyAllMesh()
{
	// 例外処理
	if (Mesh.size() == 0)
		return true;

	for (INT i = 0; i < (INT)Mesh.size(); i++)
	{
		SAFE_DELETE(Mesh[i]->pAttr);
		SAFE_DELETE(Mesh[i]->pIndx);
		SAFE_DELETE(Mesh[i]->pVtx);
		SAFE_DELETE(Mesh[i]->pIndx);
		SAFE_RELEASE(Mesh[i]->pD3DXMesh);
		SAFE_RELEASE(Mesh[i]->pD3DXBuffMat);
	//	Mesh.erase(Mesh.begin() + i);
	}

	Mesh.clear();
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::DestroyHierarchyMesh(CHAR *pszChakNeme)
{
	for (INT i = HierarchyMesh.size() - 1; i >= 0; i--)
		if (strcmp(HierarchyMesh[i]->szMeshFileName, pszChakNeme) == 0)
		{
			
			// アニメーション解放
			if (HierarchyMesh[i]->ppAnimSet)
			{
				for (UINT j = 0; j < HierarchyMesh[i]->uNumAnimSet; j++)
					SAFE_RELEASE(HierarchyMesh[i]->ppAnimSet[j]);
				SAFE_DELETE_ARRAY(HierarchyMesh[i]->ppAnimSet);
			}
			SAFE_RELEASE(HierarchyMesh[i]->pAnimCtrl);

			// メッシュ解放
			if (HierarchyMesh[i]->pFrameRoot)
			{
				D3DXFrameDestroy(HierarchyMesh[i]->pFrameRoot, &HierarchyMesh[i]->Hierarchy);
				HierarchyMesh[i]->pFrameRoot = NULL;
			}
			
			return true;
		}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 全階層構造用メッシュを解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::DestroyAllHierarchyMesh()
{
	for (INT i = HierarchyMesh.size() - 1; i >= 0; i--)
	{
		// アニメーション解放
		if (HierarchyMesh[i]->ppAnimSet)
		{
			for (UINT j = 0; j < HierarchyMesh[i]->uNumAnimSet; j++)
				SAFE_RELEASE(HierarchyMesh[i]->ppAnimSet[j]);
			SAFE_DELETE_ARRAY(HierarchyMesh[i]->ppAnimSet);
		}
		SAFE_RELEASE(HierarchyMesh[i]->pAnimCtrl);

		// メッシュ解放
		if (HierarchyMesh[i]->pFrameRoot)
		{
			D3DXFrameDestroy(HierarchyMesh[i]->pFrameRoot, &HierarchyMesh[i]->Hierarchy);
			HierarchyMesh[i]->pFrameRoot = NULL;
		}

	}
	HierarchyMesh.clear();
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::DestroyAllTexture()
{
	// 例外処理
	if (Texture.size() <= 0)
		return false;

	for (UINT i = 0; i < Texture.size(); i++)
	{
		SAFE_RELEASE(Texture[i]->pD3DTexture);
		Texture.erase(Texture.begin() + i);
	}

	Texture.clear();

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報解放(フェード用以外)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::DestroyVtx()
{
	// 例外処理
	if (VtxBoard.size() <= 0)
		return false;

	// 頂点情報解放
	for (UINT i = 0; i < VtxBoard.size(); i++)
		SAFE_RELEASE(VtxBoard[i]->pD3DVtxBuffBoard);

	VtxBoard.clear();
	
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード用頂点情報解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_RESOURCE_MANAGER::DestroyFadeVtx()
{
	// 例外処理
	if (VtxFadeBoard.size() <= 0)
		return false;

	// 頂点情報解放
	for (UINT i = 0; i < VtxFadeBoard.size(); i++)
		SAFE_RELEASE(VtxFadeBoard[i]->pD3DVtxBuffBoard);
	VtxFadeBoard.clear();

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ作成(フェード)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_RESOURCE_MANAGER::CreateFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{
	// ファイル名セット
	strcpy(TextureData.szTexFileName, pszFilename);

	FadeTexture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, FadeTexture.back()->szTexFileName, &FadeTexture.back()->pD3DTexture))
		return;
}

void  C_RESOURCE_MANAGER::CreateNormalTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{

	// 既に生成されているか
	if (CheckExisting(pszFilename, TextureData))
		return;

	// ファイル名セット
	strcpy(TextureData.szTexFileName, pszFilename);

	Texture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, Texture.back()->szTexFileName, &Texture.back()->pD3DTexture))
		return ;
}