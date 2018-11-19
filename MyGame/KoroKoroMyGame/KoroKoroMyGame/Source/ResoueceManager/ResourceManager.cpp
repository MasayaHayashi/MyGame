//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_ResourceManager.cpp
// リソース管理(メッシュ、テクスチャ管理)
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "ResourceManager.h"
#include <string>
#include <tchar.h>
#include <utility>
#include "../CrtDebug/CrtDebug.h"
#include "../Pawn/Pawn.h"
#include "../Board/Board.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Pawn/Pawn.h"


// ===== 静的メンバ変数 =====
std::unique_ptr<ResourceManager>    ResourceManager::instancePtr = nullptr;
std::vector <MeshData*>				ResourceManager::meshes;
std::vector<TEXTURE_DATA*>			ResourceManager::texture;				// テクスチャ情報
std::vector<TEXTURE_DATA*>			ResourceManager::fadeTexture;			// テクスチャ情報
std::vector<VERTEX_BOARD_DATA*>		ResourceManager::vtxBoard;				// ボード頂点情報
std::vector<VERTEX_BOARD_DATA*>		ResourceManager::vtxFadeBoard;			// ボード頂点情報
std::unordered_map<std::string, HIERARCHY_MESH_DATA*>	ResourceManager::hierarchyMesh;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ResourceManager::ResourceManager()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ResourceManager::~ResourceManager()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデル生成(階層構造無し)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::makeModel(MeshData &meshData, CHAR *pszFilename, MeshObjType &umeshType)
{
	// 例外処理
	if (!pszFilename)
	{
		return E_FAIL;
	}

	// メッシュの種類
	umeshType = MeshObjType::NormalModel;

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();

	// 既に生成されているか
	if (checkExisting(pszFilename, &meshData))
	{
		return S_OK;
	}

	// ファイル名セット
	strcpy_s(meshData.meshFileName, pszFilename);

	// メッシュ情報セット

	meshes.push_back(&meshData);
	
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(pszFilename, D3DXMESH_SYSTEMMEM, devicePtr, nullptr, &meshes.back()->materialBufferPtr, nullptr, &meshes.back()->numMat, &meshes.back()->meshPtr)))
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("モデル読み込みエラー"), MB_ICONERROR);
		return E_FAIL;
	}

	// 属性取得のための下準備
	LPD3DXMESH pmeshWk;
	HRESULT hr = meshes.back()->meshPtr->Optimize(D3DXMESHOPT_ATTRSORT, nullptr, nullptr, nullptr, nullptr, &pmeshWk);

	if (SUCCEEDED(hr))
	{
		//meshes.back()->meshPtr->Release();
		meshes.back()->meshPtr = pmeshWk;
	}
	else
	{
		//SAFE_RELEASE(meshes.back()->materialBufferPtr);	// マテリアル解放
		return false;
	}

	// 属性テーブル取得
	meshes.back()->dwAttrNum = 0;
	meshes.back()->meshPtr->GetAttributeTable(nullptr, &meshes.back()->dwAttrNum);
	meshes.back()->attrPtr.reset(NEW D3DXATTRIBUTERANGE[meshes.back()->dwAttrNum]);
	meshes.back()->meshPtr->GetAttributeTable(meshes.back()->attrPtr.get(), &meshes.back()->dwAttrNum);

	// 指定の頂点フォーマットに変換
	DWORD dwFVF = meshes.back()->meshPtr->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pmeshTmp = meshes.back()->meshPtr.Get();
		pmeshTmp->CloneMeshFVF(pmeshTmp->GetOptions(), FVF_TVERTEX, devicePtr, &meshes.back()->meshPtr);
		delete pmeshTmp;
	//	SAFE_RELEASE(pmeshTmp);
		// 法線が無い場合は強制的に追加
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(meshes.back()->meshPtr.Get(), nullptr);
		}
	}

	// 頂点情報サイズ数取得
	meshes.back()->dwNumVtx = meshes.back()->meshPtr->GetNumVertices();	// 頂点数取得
	meshes.back()->vertexPtr.reset(NEW MESH_VTX[meshes.back()->dwNumVtx]);
	LPVOID pWork;

	// 頂点バッファアンロック
	meshes.back()->meshPtr->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(meshes.back()->vertexPtr.get(), pWork, sizeof(MESH_VTX) * meshes.back()->dwNumVtx);

	// 頂点バッファロック
	meshes.back()->meshPtr->UnlockVertexBuffer();

	// インデックスバッファから読み込み
	meshes.back()->dwNumIndx = meshes.back()->meshPtr->GetNumFaces() * 3;
	meshes.back()->indexPtr.reset(NEW WORD[meshes.back()->dwNumIndx]);

	// インデックスバッファロック
	meshes.back()->meshPtr->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(meshes.back()->indexPtr.get(), pWork, sizeof(WORD) * meshes.back()->dwNumIndx);

	// インデックスバッファアンロック
	meshes.back()->meshPtr->UnlockIndexBuffer();

	// 境界ボックスと中心座標を求める
	meshes.back()->maxVtx = meshes.back()->vertexPtr.get()[0].VtxPos;
	meshes.back()->minVtx = meshes.back()->vertexPtr.get()[0].VtxPos;
	for (WORD i = 1; i < meshes.back()->dwNumVtx; i++)
	{
		// 最大値取得
		if (meshes.back()->maxVtx.x < meshes.back()->vertexPtr.get()[i].VtxPos.x)
		{
			meshes.back()->maxVtx.x = meshes.back()->vertexPtr.get()[i].VtxPos.x;
		}

		if (meshes.back()->maxVtx.y < meshes.back()->vertexPtr.get()[i].VtxPos.y)
		{
			meshes.back()->maxVtx.y = meshes.back()->vertexPtr.get()[i].VtxPos.y;
		}

		if (meshes.back()->maxVtx.z < meshes.back()->vertexPtr.get()[i].VtxPos.z)
		{
			meshes.back()->maxVtx.z = meshes.back()->vertexPtr.get()[i].VtxPos.z;
		}

		// 最小値取得
		if (meshes.back()->minVtx.x > meshes.back()->vertexPtr.get()[i].VtxPos.x)
		{
			meshes.back()->minVtx.x = meshes.back()->vertexPtr.get()[i].VtxPos.x;
		}

		if (meshes.back()->minVtx.y > meshes.back()->vertexPtr.get()[i].VtxPos.y)
		{
			meshes.back()->minVtx.y = meshes.back()->vertexPtr.get()[i].VtxPos.y;
		}
		if (meshes.back()->minVtx.z > meshes.back()->vertexPtr.get()[i].VtxPos.z)
		{
			meshes.back()->minVtx.z = meshes.back()->vertexPtr.get()[i].VtxPos.z;
		}
	}

	// 中心座標、あたり判定取得
	meshes.back()->centerPos = (meshes.back()->maxVtx + meshes.back()->minVtx) * 0.5f;
	meshes.back()->collitionBox = (meshes.back()->maxVtx - meshes.back()->minVtx) * 0.5f;

	// 境界球取得
	FLOAT dx = meshes.back()->vertexPtr.get()[0].VtxPos.x - meshes.back()->centerPos.x;
	FLOAT dy = meshes.back()->vertexPtr.get()[0].VtxPos.y - meshes.back()->centerPos.y;
	FLOAT dz = meshes.back()->vertexPtr.get()[0].VtxPos.z - meshes.back()->centerPos.z;
	meshes.back()->collisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < meshes.back()->dwNumVtx; i++)
	{
		dx = meshes.back()->vertexPtr.get()[i].VtxPos.x - meshes.back()->centerPos.x;
		dy = meshes.back()->vertexPtr.get()[i].VtxPos.y - meshes.back()->centerPos.y;
		dz = meshes.back()->vertexPtr.get()[i].VtxPos.z - meshes.back()->centerPos.z;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (meshes.back()->collisionRadus < fRadius)
		{
			meshes.back()->collisionRadus = fRadius;
		}
	}

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::makevertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszName)
{
	// 識別用ファイル名セット
	strcpy_s(VtxBordData.name, pszName);

	// フェード用判定
	if (VtxBordData.fade)
	{
		makeVtxFade(VtxBordData);
	}
	else
	{
		makeVtx(VtxBordData);
	}

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::makeVtx(VERTEX_BOARD_DATA &vtxBoardData)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 頂点情報情報セット
	vtxBoard.push_back(&vtxBoardData);

	// 頂点情報生成
	switch (vtxBoardData.boardType)
	{
	case  boardType::Polygon2d:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (DirectX3D::VertexSize),	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y + vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y + vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y - vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y - vtxBoard.back()->size.y * 0.5f, 0.0f);

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
			vtxBoard.back()->pD3DVtxBuff->Unlock();
		}
		return true;
	case boardType::Billboard:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_3D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
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

		// 頂点データをアンロックする
		vtxBoard.back()->pD3DVtxBuff->Unlock();
	}
		return true;
	case boardType::Polygon3d:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&vtxBoardData.pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))						// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
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
		vtxBoard.back()->pD3DVtxBuff->Unlock();

		return true;
	default:
		break;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード用頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::makeVtxFade(VERTEX_BOARD_DATA &vtxBoardData)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 頂点情報情報セット
	vtxFadeBoard.push_back(&vtxBoardData);

	// 頂点情報生成
	switch (vtxBoardData.boardType)
	{
	case boardType::Polygon2d:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxFadeBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y + vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y + vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y - vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y - vtxFadeBoard.back()->size.y * 0.5f, 0.0f);

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
			vtxFadeBoard.back()->pD3DVtxBuff->Unlock();
		}
		return true;
	case boardType::Billboard:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_3D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxFadeBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		
		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
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

		// 頂点データをアンロックする
		vtxFadeBoard.back()->pD3DVtxBuff->Unlock();
	}
		return true;

	case boardType::Polygon3d:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&vtxFadeBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))						// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
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

		// 頂点データをアンロックする
		vtxFadeBoard.back()->pD3DVtxBuff->Unlock();

		return true;
	default:
		break;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::createTexture(TEXTURE_DATA &TextureData,  CHAR *pszFilename)
{
	// 例外処理
	if (!pszFilename)
	{
		return E_FAIL;
	}

	// 既に生成されているか
	if (checkExisting(pszFilename, TextureData))
	{
		return S_OK;
	}

	// ファイル名セット
	strcpy_s(TextureData.texFileName, pszFilename);

	texture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, texture.back()->texFileName, &texture.back()->pD3DTexture))
	{
		return S_OK;
	}
	else
	{
	//	MessageBox(nullptr, TEXT("テクスチャ読み込みエラー"), TEXT("Error"), MB_ICONERROR);
		return E_FAIL;
	}
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::makeModelHierarchy(HIERARCHY_MESH_DATA &setHierarchyMedhData,CHAR *pszFilename,std::string keyName, MeshObjType &umeshType)
{
	// メッシュの種類初期化
	umeshType = MeshObjType::HierarchyModel;

	// メッシュ情報セット
	strcpy_s(setHierarchyMedhData.meshFileName, pszFilename);	// ファイル名セット

	// メッシュ情報セット
	std::pair<std::string, HIERARCHY_MESH_DATA*> setPair = std::make_pair(keyName, &setHierarchyMedhData);
	hierarchyMesh.insert(setPair);

	// ディレクトリ抽出
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath_s(hierarchyMesh[keyName]->meshFileName, szDir, sizeof(szDir), szDirWk, sizeof(szDirWk), nullptr, 0, nullptr, 0);
	lstrcat(szDir, szDirWk);
	hierarchyMesh[keyName]->hierarchy.setDirectory(szDir);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 階層構造メッシュの読み込み
	HRESULT hr = D3DXLoadMeshHierarchyFromX(hierarchyMesh[keyName]->meshFileName, D3DXMESH_MANAGED, devicePtr, &hierarchyMesh[keyName]->hierarchy, nullptr, &hierarchyMesh[keyName]->frameRoot, &hierarchyMesh[keyName]->animCtrlPtr);
	if (FAILED(hr))
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("階層メッシュ読み込みエラー"), MB_ICONERROR);
		return false;
	}

	// ボーンとフレームの関連付け
	hr = allocAllBoneMatrix(hierarchyMesh[keyName]->frameRoot,pszFilename,keyName);
	if (FAILED(hr))
	{
		return false;
	}

	// アニメーションセット取得
	hierarchyMesh[keyName]->numAnimset = 0;
	if (hierarchyMesh[keyName]->animCtrlPtr)
	{
		hierarchyMesh[keyName]->numAnimset = hierarchyMesh[keyName]->animCtrlPtr->GetNumAnimationSets();
		if (hierarchyMesh[keyName]->numAnimset > 0)
		{
			hierarchyMesh[keyName]->pAnimSetPtr.reset(NEW LPD3DXANIMATIONSET[hierarchyMesh[keyName]->numAnimset]);

			for (DWORD u = 0; u < hierarchyMesh[keyName]->numAnimset; u++)
			{
				hierarchyMesh[keyName]->animCtrlPtr->GetAnimationSet(u, &hierarchyMesh[keyName]->pAnimSetPtr.get()[u]);

			}
		}
	}

	if (hierarchyMesh[keyName]->frameRoot)
	{
		// マトリックス更新
		setTime(0.0,pszFilename,keyName);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		updateFrameMatrices(hierarchyMesh[keyName]->frameRoot, &world);

		// 境界球/境界ボックス取得
		calcCollision(hierarchyMesh[keyName]->frameRoot,pszFilename,keyName);
	}

	// 経過時間計測用時刻設定
	hierarchyMesh[keyName]->dwPrev = timeGetTime();

	return SUCCEEDED(hr);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレームのマトリックスを更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	// 親のマトリックスを掛け合わせる
	if (pParentMatrix)
	{
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	}
	else
	{
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	// 兄弟フレームがあればマトリックスを更新
	if (pFrame->pFrameSibling)
	{
		updateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	// 子フレームがあればマトリックスを更新
	if (pFrame->pFrameFirstChild)
	{
		// 自分が親となるので、自分のマトリックスを渡す
		updateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->combinedTransformationMatrix);
	}
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメーション開始時間設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::setTime(DOUBLE dTime,CHAR *pszFilename,std::string keyName)
{
	if (hierarchyMesh[keyName]->animCtrlPtr == nullptr)
	{
		return;
	}

	for (DWORD i = 0; i < hierarchyMesh[keyName]->animCtrlPtr->GetMaxNumTracks(); ++i)
	{
		hierarchyMesh[keyName]->animCtrlPtr->SetTrackPosition(i, 0);
	}

	hierarchyMesh[keyName]->animCtrlPtr->ResetTime();
	hierarchyMesh[keyName]->animCtrlPtr->AdvanceTime(dTime, nullptr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::allocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename,std::string keyName)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrBase;

	if (meshContainerPtr->pSkinInfo == nullptr)
	{
		return S_OK;	// スキン情報が無ければ何もしない
	}

	DWORD dwBoneNum = meshContainerPtr->pSkinInfo->GetNumBones();
	meshContainerPtr->ppBoneMatrix = NEW LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(hierarchyMesh[keyName]->frameRoot, meshContainerPtr->pSkinInfo->GetBoneName(i));

		if (pFrame == nullptr)
		{
			return E_FAIL;
		}

		meshContainerPtr->ppBoneMatrix[i] = &pFrame->combinedTransformationMatrix;
	}
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::allocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename,std::string keyName)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer)
	{
		hr = allocBoneMatrix(pFrame->pMeshContainer,pszFilename,keyName);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pFrame->pFrameSibling)
	{
		hr = allocAllBoneMatrix(pFrame->pFrameSibling,pszFilename,keyName);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pFrame->pFrameFirstChild)
	{
		hr = allocAllBoneMatrix(pFrame->pFrameFirstChild, pszFilename, keyName);
	}

	return hr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//  階層構造用 境界球/境界ボックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::calcCollision(LPD3DXFRAME pFrame,CHAR *pszFilename,std::string keyName)
{
	D3DXVECTOR3& vMax = hierarchyMesh[keyName]->collitionBox;
	D3DXVECTOR3& vMin = hierarchyMesh[keyName]->centerPos;

	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	
	hierarchyMesh[keyName]->collisionRadus = -1.0f;
	calcCollisionFrame(pFrame,pszFilename,keyName);
	
	D3DXVECTOR3 vBBox, vCenter;
	vBBox = (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	
	hierarchyMesh[keyName]->collitionBox = vBBox;
	hierarchyMesh[keyName]->centerPos = vCenter;
	hierarchyMesh[keyName]->collisionRadus = 0.0f;
	calcCollisionFrame(pFrame,pszFilename,keyName);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレーム毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::calcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName)
{
	LPD3DXMESHCONTAINER meshContainerPtr = pFrame->pMeshContainer;
	while (meshContainerPtr)
	{
		// メッシュコンテナ毎の頂点座標の抽出
		calcCollisionMeshContainer(meshContainerPtr, pFrame,pszFilename,keyName);
		// 次のメッシュコンテナ
		meshContainerPtr = meshContainerPtr->pNextMeshContainer;
	}
	// 兄弟フレームがあれば兄弟フレームを描画
	if (pFrame->pFrameSibling)
	{
		calcCollisionFrame(pFrame->pFrameSibling, pszFilename, keyName);
	}
	// 子フレームがあれば子フレームを描画
	if (pFrame->pFrameFirstChild)
	{
		calcCollisionFrame(pFrame->pFrameFirstChild, pszFilename, keyName);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュコンテナ毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::calcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->combinedTransformationMatrix;
	// 頂点座標の抽出
	LPD3DXMESH pmesh = meshContainerPtr->MeshData.pMesh;
	DWORD dwStride = pmesh->GetNumBytesPerVertex();
	DWORD dwVtx = pmesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pmesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (hierarchyMesh[keyName]->collisionRadus < 0.0f)
		{
			D3DXVECTOR3& vMax = hierarchyMesh[keyName]->collitionBox;
			D3DXVECTOR3& vMin = hierarchyMesh[keyName]->centerPos;
			if (vMax.x < vtx.x) { vMax.x = vtx.x; }
			if (vMax.y < vtx.y) { vMax.y = vtx.y; }
			if (vMax.z < vtx.z) { vMax.z = vtx.z; }
			if (vMin.x > vtx.x) { vMin.x = vtx.x; }
			if (vMin.y > vtx.y) { vMin.y = vtx.y; }
			if (vMin.z > vtx.z) { vMin.z = vtx.z; }
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - hierarchyMesh[keyName]->centerPos));
			if (hierarchyMesh[keyName]->collisionRadus < fRadius)
			{
				hierarchyMesh[keyName]->collisionRadus = fRadius;
			}
		}
	}
	pmesh->UnlockVertexBuffer();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::checkExisting(CHAR *pszChakNeme,MeshData *meshDataPtr)
{


	// 全メッシュを検索
	for (UINT i = 0; i < meshes.size(); i++)
	{
		if (strcmp(meshes[i]->meshFileName, pszChakNeme) == 0)
		{
			meshDataPtr = meshes[i];	// 生成済みなら元のデータを使用
			return true;
		}
	}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::checkExisting(CHAR *pszChakNeme, TEXTURE_DATA &TextureData)
{
	// 全テクスチャを検索
	for (UINT i = 0; i < texture.size(); i++)
	{
		if (strcmp(texture[i]->texFileName, pszChakNeme) == 0)
		{
			TextureData = *texture[i];	// 生成済みなら元のデータを使用
			return true;
		}
	}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::checkExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &TextureData)
{
	// 全テクスチャを検索
	for (UINT i = 0; i < vtxBoard.size(); i++)
	{
		if (strcmp(vtxBoard[i]->name, pszChakNeme) == 0)
		{
			TextureData = *vtxBoard[i];	// 生成済みなら元のデータを使用
			return true;
		}
	}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyMesh(CHAR *pszChakNeme)
{
	/*
	for (INT i = (INT)meshes.size() - 1; i >= 0; i--)
	{
		if (strcmp(meshes[i]->meshFileName, pszChakNeme) == 0)
		{
			SAFE_DELETE(meshes[i]->attrPtr);
			SAFE_DELETE(meshes[i]->pIndx);
			SAFE_DELETE(meshes[i]->pVtx);
			meshes.erase(meshes.begin() + i);
			return true;
		}
	
	}
	*/
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 全メッシュ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyAllMesh()
{
	/*
	// 例外処理
	if (meshes.size() == 0)
		return true;

	for (INT i = 0; i < (INT)meshes.size(); i++)
	{
		SAFE_DELETE(meshes[i]->attrPtr);
		SAFE_DELETE(meshes[i]->pIndx);
		SAFE_DELETE(meshes[i]->pVtx);
		SAFE_DELETE(meshes[i]->pIndx);

	//	meshes.erase(meshes.begin() + i);
	}

	meshes.clear();
	*/
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyHierarchymesh(CHAR *pszChakNeme,std::string keyName)
{
	
	// アニメーション解放
	if (hierarchyMesh[keyName]->pAnimSetPtr)
	{
		for (UINT j = 0; j < hierarchyMesh[keyName]->numAnimset; j++)
		{
			hierarchyMesh[keyName]->pAnimSetPtr.get()[j]->Release();
			hierarchyMesh[keyName]->pAnimSetPtr.get()[j] = nullptr;
		}
	}
	else
	{
		return false;
	}

	hierarchyMesh[keyName]->animCtrlPtr->Release();
	hierarchyMesh[keyName]->animCtrlPtr = nullptr;

//	SAFE_RELEASE(hierarchyMesh[i]->animCtrlPtr);

	// メッシュ解放
	if (hierarchyMesh[keyName]->frameRoot)
	{
		D3DXFrameDestroy(hierarchyMesh[keyName]->frameRoot, &hierarchyMesh[keyName]->hierarchy);
		hierarchyMesh[keyName]->frameRoot = nullptr;

		hierarchyMesh.clear();

		return true;
	}
	else
	{
		return false;
	}

	
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 全階層構造用メッシュを解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyAllHierarchymesh()
{
	/*
	for (INT i = hierarchyMesh.size() - 1; i >= 0; i--)
	{
		// アニメーション解放
		if (hierarchyMesh[i]->ppAnimSet)
		{
			for (UINT j = 0; j < hierarchyMesh[i]->numAnimset; j++)
				SAFE_RELEASE(hierarchyMesh[i]->ppAnimSet[j]);
			SAFE_DELETE_ARRAY(hierarchyMesh[i]->ppAnimSet);
		}
		SAFE_RELEASE(hierarchyMesh[i]->animCtrlPtr);

		// メッシュ解放
		if (hierarchyMesh[i]->frameRoot)
		{
			D3DXFrameDestroy(hierarchyMesh[i]->frameRoot, &hierarchyMesh[i]->hierarchy);
			hierarchyMesh[i]->frameRoot = nullptr;
		}

	}
	hierarchyMesh.clear();
	return true;
	*/

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyAllTexture()
{
	/*
	// 例外処理
	if (texture.size() <= 0)
		return false;

	for (UINT i = 0; i < texture.size(); i++)
	{
		SAFE_RELEASE(texture[i]->pD3DTexture);
		texture.erase(texture.begin() + i);
	}

	texture.clear();

	*/
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報解放(フェード用以外)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyVtx()
{
	/*
	// 例外処理
	if (vtxBoard.size() <= 0)
		return false;

	// 頂点情報解放
	for (UINT i = 0; i < vtxBoard.size(); i++)
		SAFE_RELEASE(vtxBoard[i]->pD3DVtxBuff);

	vtxBoard.clear();
	
	*/
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード用頂点情報解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyFadeVtx()
{
	/*
	// 例外処理
	if (vtxFadeBoard.size() <= 0)
		return false;

	// 頂点情報解放
	for (UINT i = 0; i < vtxFadeBoard.size(); i++)
		SAFE_RELEASE(vtxFadeBoard[i]->pD3DVtxBuff);
	vtxFadeBoard.clear();

	*/
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ作成(フェード)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::createFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{
	// ファイル名セット
	strcpy_s(TextureData.texFileName, pszFilename);

	fadeTexture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, fadeTexture.back()->texFileName, &fadeTexture.back()->pD3DTexture))
		return;
}

void  ResourceManager::createNormalTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{

	// 既に生成されているか
	if (checkExisting(pszFilename, TextureData))
		return;

	// ファイル名セット
	strcpy_s(TextureData.texFileName, pszFilename);

	texture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, texture.back()->texFileName, &texture.back()->pD3DTexture))
		return ;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// インスタンス生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::createInstance()
{
	if (instancePtr.get() == nullptr)
	{
		instancePtr.reset(NEW ResourceManager());

		return true;
	}
	else
	{
		return false;
	}
}