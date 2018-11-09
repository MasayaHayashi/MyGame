//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Pawn.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Pawn.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Collider/Collider.h"
#include "../Figure/Figure.h"
#include <tchar.h>
#include "../KeyBoard/Keyboard.h"

// ===== グローバル変数宣言 =====
#define SAFE_DELETE(p)       { if(p!=nullptr) { delete (p);     (p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p!=nullptr) { (p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; }}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Pawn::Pawn()
{
	// 描画関連初期化
	cross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	oldRadRot = 0.0f;

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	// 各種ポインタ初期化
	meshData.pVtx			= nullptr;
	meshData.pIndx			= nullptr;
	meshData.pAttr			= nullptr;
	textureData.pD3DTexture = nullptr;
	meshData.pD3DXMesh		= nullptr;
	meshData.pD3DXBuffMat	= nullptr;

	// 位置・向きの初期設定
	pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	accele			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	collisionSize	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	destLanding		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	quatanion	= D3DXQUATERNION(0, 0, 0, 0);
	destQua		= D3DXQUATERNION(0, 0, 0, 0);

	speed = 0.0f;

	hitIndex = -1;

	collisionRadus = 0.0f;
	colorAlpha		= 1.0f;
	maxTrackTime	= 0.0f;

	curSelectAnim	= 0;
	endAnim			= false;
	isHit			= false;
	isGround		= false;

	currentAnim	= 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ(識別用添え字番号指定)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Pawn::Pawn(UINT SetuIndxNum)
{
	// 描画関連初期化
//	pD3DXMesh = nullptr;

	// 位置・向きの初期設定
	pos			  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale		  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	move		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot			  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	collisionSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	collisionRadus  = 0.0f;
	colorAlpha		 = 1.0f;
	maxTrackTime	 = 0.0f;

	dwNumIndx		 = 0;
	curSelectAnim	 = 0;
	endAnim		 = false;

	idNumber = SetuIndxNum;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Pawn::~Pawn()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::initialize()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::finalize()
{
	ResourceManager::destroyAllMesh();
	ResourceManager::destroyAllTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ポーン更新(共通処理)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::updateObject()
{

}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::drawObject()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// 例外処理
	if (!isUsed)
		return;

	// 移動、回転、拡大縮小用行列
	D3DXMATRIX		mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL	*pD3DXMat;
	D3DMATERIAL9	matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&mtxScale);

	// 拡大縮小
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &rotMtx);

	// 移動を反映
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &worldMtx);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshData.numMat); nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, textureData.pD3DTexture);

		// 描画
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::drawObject(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// 例外処理
	if (!isUsed)
		return;

	// 移動、回転、拡大縮小用行列
	D3DXMATRIX		mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL	*pD3DXMat;
	D3DMATERIAL9	matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&mtxScale);

	// 拡大縮小
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &rotMtx);

	// 移動を反映
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);
	
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &worldMtx);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
 	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();


	LPD3DXEFFECT pEffect = DirectX3D::getEffect();

	// 
	pEffect->SetTechnique("tecMinimum");

	D3DXMATRIX mtxAll = worldMtx * mtxView * mtxProj;
	pEffect->SetMatrix("mWVP", &mtxAll);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (int nCntMat = 0; nCntMat < (int)meshData.numMat; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, textureData.pD3DTexture);

		// 描画
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	pEffect->EndPass();
	pEffect->End();

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::drawObject(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTex, LPD3DXBUFFER pBuff,DWORD uNumMat)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// 移動、回転、拡大縮小用行列
	D3DXMATRIX mtxRot, translateMtx, mtxScale;

	//	D3DXMATRIX mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&mtxScale);

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// 拡大縮小
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &rotMtx);

	// 移動を反映
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &worldMtx);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshData.numMat); nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, textureData.pD3DTexture);

		// 描画
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 現在自身が持っているワールド行列をそのまま使い描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::drawObjectLocal()
{
	// 例外処理
	if (!isUsed)
		return;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//	D3DXMATRIX mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &worldMtx);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)meshData.numMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, textureData.pD3DTexture);

		// 描画
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレームの描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::drawFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;

	while (pMeshContainer)
	{
		// メッシュコンテナの描画
		RenderMeshContainer(pMeshContainer, pFrame);
		// 次のメッシュコンテナ
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	// 兄弟フレームがあれば兄弟フレームを描画
	if (pFrame->pFrameSibling)
		drawFrame(pFrame->pFrameSibling);
	// 子フレームがあれば子フレームを描画
	if (pFrame->pFrameFirstChild)
		drawFrame(pFrame->pFrameFirstChild);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュコンテナの描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::RenderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	if (pMeshContainer->pSkinInfo)
	{
		// スキンメッシュモデル
		LPD3DXBONECOMBINATION pBoneCombi = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		DWORD dwPrevBoneID = UINT_MAX;
		DWORD dwBlendMatrix;
		for (DWORD i = 0; i < pMeshContainer->dwBone; i++)
		{
			dwBlendMatrix = 0;
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k)
			{
				if (pBoneCombi[i].BoneId[k] != UINT_MAX)
					dwBlendMatrix = k;
			}
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrix);
			for (DWORD k = 0; k < pMeshContainer->dwWeight; k++)
			{
				DWORD id = pBoneCombi[i].BoneId[k];
				if (id != UINT_MAX) 
				{
					pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &(pMeshContainer->pBoneOffsetMatrix[id] * *pMeshContainer->ppBoneMatrix[id]));
				}
			}
			pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombi[i].AttribId].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneCombi[i].AttribId]);
			dwPrevBoneID = pBoneCombi[i].AttribId;

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		return;
	}
	// スキン無しモデル
	pDevice->SetTransform(D3DTS_WORLD, &pFrame->combinedTransformationMatrix);
	for (DWORD iAttrib = 0; iAttrib < pMeshContainer->NumMaterials; iAttrib++)
	{
		DWORD dwAttrib = pMeshContainer->pAttributeTable[iAttrib].AttribId;
		pDevice->SetMaterial(&pMeshContainer->pMaterials[dwAttrib].MatD3D);
		pDevice->SetTexture(0, pMeshContainer->ppTextures[dwAttrib]);
		pMeshContainer->MeshData.pMesh->DrawSubset(dwAttrib);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 平行移動セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setTranslate(D3DXVECTOR3 pos)
{
	// 移動を反映
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);

	D3DXMatrixInverse(&mtxInverse, nullptr, &translateMtx);	// 逆行列生成
	mtxOldTranslate = translateMtx;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデル生成(階層構造無し)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Pawn::makeModel()
{
	// 例外処理
	if (!fileName)
		return E_FAIL;
//	if (pD3DXMesh)
//		return E_FAIL;

	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, pDevice, nullptr, &pD3DXBuffMat, nullptr, &uNumMat, &pD3DXMesh)))
	{
		return E_FAIL;
	}

	// 属性取得のための下準備
	LPD3DXMESH pMeshWk;
	HRESULT hr = pD3DXMesh->Optimize(D3DXMESHOPT_ATTRSORT,nullptr, nullptr, nullptr, nullptr, &pMeshWk);

	if (SUCCEEDED(hr)) 
	{
		pD3DXMesh->Release();
		pD3DXMesh = pMeshWk;
	}
	else
	{
		SAFE_RELEASE(pD3DXBuffMat);
		return false;
	}

	// 属性テーブル取得
	dwAttrNum = 0;
	pD3DXMesh->GetAttributeTable(nullptr, &dwAttrNum);
	pAttr = new D3DXATTRIBUTERANGE[dwAttrNum];
	pD3DXMesh->GetAttributeTable(pAttr, &dwAttrNum);

	// 指定の頂点フォーマットに変換
	DWORD dwFVF = pD3DXMesh->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pMeshTmp = pD3DXMesh;
		pMeshTmp->CloneMeshFVF(pMeshTmp->GetOptions(), FVF_TVERTEX, pDevice, &pD3DXMesh);
		SAFE_RELEASE(pMeshTmp);
		// 法線が無い場合は強制的に追加
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(pD3DXMesh, nullptr);
		}
	}


	// 頂点情報サイズ数取得
	dwNumVtx = pD3DXMesh->GetNumVertices();	// 頂点数取得
	pVtx = new MESH_VTX[dwNumVtx];
	LPVOID pWork;

	// 頂点バッファアンロック
	pD3DXMesh->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(pVtx, pWork, sizeof(MESH_VTX) * dwNumVtx);

	// 頂点バッファロック
	pD3DXMesh->UnlockVertexBuffer();

	// インデックスバッファから読み込み
	dwNumIndx = pD3DXMesh->GetNumFaces() * 3;
	pIndx = new WORD[dwNumIndx];

	// インデックスバッファロック
	pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(pIndx, pWork, sizeof(WORD) * dwNumIndx);

	// インデックスバッファアンロック
	pD3DXMesh->UnlockIndexBuffer();

	// 境界ボックスと中心座標を求める
	maxVtx = pVtx[0].VtxPos;
	minVtx = pVtx[0].VtxPos;
	for (WORD i = 1; i < dwNumVtx; i++)
	{
		// 最大値取得
		if (maxVtx.x < pVtx[i].VtxPos.x)
			maxVtx.x = pVtx[i].VtxPos.x;

		if (maxVtx.y < pVtx[i].VtxPos.y)
			maxVtx.y = pVtx[i].VtxPos.y;

		if (maxVtx.z < pVtx[i].VtxPos.z)
			maxVtx.z = pVtx[i].VtxPos.z;

		// 最小値取得
		if (minVtx.x > pVtx[i].VtxPos.x)
			minVtx.x = pVtx[i].VtxPos.x;

		if (minVtx.y > pVtx[i].VtxPos.y)
			minVtx.y = pVtx[i].VtxPos.y;

		if (minVtx.z > pVtx[i].VtxPos.z)
			minVtx.z = pVtx[i].VtxPos.z;
	}

	// 最大、最小値をスケールに合わせる


	// 中心座標、あたり判定取得
	centerPos	 = (maxVtx + minVtx) * 0.5f;
	collitionBox = (maxVtx - minVtx) * 0.5f;

	// 境界球取得
	float dx = pVtx[0].VtxPos.x - centerPos.x;
	float dy = pVtx[0].VtxPos.y - centerPos.y;
	float dz = pVtx[0].VtxPos.z - centerPos.z;
	collisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < dwNumVtx; i++) 
	{
		dx = pVtx[i].VtxPos.x - centerPos.x;
		dy = pVtx[i].VtxPos.y - centerPos.y;
		dz = pVtx[i].VtxPos.z - centerPos.z;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (collisionRadus < fRadius)
			collisionRadus = fRadius;
	}

	// 中心座標をワールド座標系に変換
	centerPos += pos;

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Pawn::makeModelHierarchy()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// ディレクトリ抽出
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(fileName, szDir, szDirWk, nullptr, nullptr);
	lstrcat(szDir, szDirWk);
	Hierarchy.setDirectory(szDir);

	// 階層構造メッシュの読み込み
	HRESULT hr = D3DXLoadMeshHierarchyFromX(fileName, D3DXMESH_MANAGED, pDevice, &Hierarchy, nullptr, &pFrameRoot, &animCtrl);
	if (FAILED(hr))
		return false;

	// ボーンとフレームの関連付け
	hr = AllocAllBoneMatrix(pFrameRoot);
	if (FAILED(hr)) return false;

	// アニメーションセット取得
	numAnimset = 0;
	if (animCtrl)
	{
		numAnimset = animCtrl->GetNumAnimationSets();
		if (numAnimset > 0)
		{
			ppAnimSet = new LPD3DXANIMATIONSET[numAnimset];
			for (DWORD u = 0; u < numAnimset; u++)
			{
				animCtrl->GetAnimationSet(u, &ppAnimSet[u]);
			}
		}
	}

	if (pFrameRoot)
	{
		// マトリックス更新
		setTime(0.0);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		updateFrameMatrices(pFrameRoot, &world);

		// 境界球/境界ボックス取得
		calcCollision(pFrameRoot);
	}

	// 経過時間計測用時刻設定
	dwPrev = timeGetTime();

	return SUCCEEDED(hr);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレームのマトリックスを更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	// 親のマトリックスを掛け合わせる
	if (pParentMatrix) 
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	else 
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix;

	// 兄弟フレームがあればマトリックスを更新
	if (pFrame->pFrameSibling) 
		updateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);

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
void Pawn::setTime(DOUBLE dTime)
{
	if (animCtrl == nullptr) 
		return;

	for (DWORD i = 0; i < animCtrl->GetMaxNumTracks(); ++i) 
	{
		animCtrl->SetTrackPosition(i, 0);
	}

	animCtrl->ResetTime();
	animCtrl->AdvanceTime(dTime, nullptr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Pawn::AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	if (pMeshContainer->pSkinInfo == nullptr)
		return S_OK;	// スキン情報が無ければ何もしない

	DWORD dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(pFrameRoot,pMeshContainer->pSkinInfo->GetBoneName(i));

		if (pFrame == nullptr)
			return E_FAIL;
		pMeshContainer->ppBoneMatrix[i] = &pFrame->combinedTransformationMatrix;
	}
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Pawn::AllocAllBoneMatrix(LPD3DXFRAME pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer) 
	{
		hr = AllocBoneMatrix(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameSibling) 
	{
		hr = AllocAllBoneMatrix(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameFirstChild) 
		hr = AllocAllBoneMatrix(pFrame->pFrameFirstChild);

	return hr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Pawn::createTexture()
{
	// 例外処理
	if (!texFileName)
		return E_FAIL;

	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(pDevice, texFileName, &pD3DTexture))
		return S_OK;

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデルの解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::destroyResorceModel()
{
	ResourceManager::destroyAllMesh();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::destroyTexture()
{
	SAFE_DELETE(pD3DTexture);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::destroyResorceTexture()
{
	ResourceManager::destroyAllTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::destroyModelHierarchy()
{
	ResourceManager::destroyAllHierarchymesh();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデルの解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::destroyModel()
{
	SAFE_DELETE(pVtx);
	SAFE_DELETE(pIndx);
	SAFE_DELETE(pAttr);

	SAFE_RELEASE(pD3DTexture);
	SAFE_RELEASE(pD3DXBuffMat);
	SAFE_RELEASE(pD3DXMesh);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置座標セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setPosition(D3DXVECTOR3 SetPos)
{
	pos = SetPos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 行列に直接位置座標をセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setOffset(D3DXVECTOR3 SetOffSet)
{
	worldMtx._41 = SetOffSet.x;
	worldMtx._42 = SetOffSet.y;
	worldMtx._43 = SetOffSet.z;

	colliderPtr->UpdateCollider(worldMtx,Figure::DeaultBoxColor);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 回転セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setRotation(D3DXVECTOR3 SetRot)
{
	rot = SetRot;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// マテリアルの色セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setMaterialColorEmissive(float fRed,float Green,float Blue,float Alpha)
{
	D3DXMATERIAL *pD3DXMat;

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)uNumMat; nCntMat++, pD3DXMat++)
	{
		pD3DXMat->MatD3D.Emissive.r = fRed;
		pD3DXMat->MatD3D.Emissive.g = Green;
		pD3DXMat->MatD3D.Emissive.b = Blue;
		pD3DXMat->MatD3D.Emissive.a = Alpha;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ワールド行列取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXMATRIX* Pawn::getWorldMtx()
{
	return &worldMtx;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右方向ベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getRightVec()
{
	return D3DXVECTOR3(worldMtx._11, worldMtx._12, worldMtx._13);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 上方向ベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getUpVec()
{
	return D3DXVECTOR3(worldMtx._21, worldMtx._22, worldMtx._23);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 前方向ベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getForwardVec()
{
	return D3DXVECTOR3(worldMtx._31, worldMtx._32, worldMtx._33);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// オフセットベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getOffset()
{
	return D3DXVECTOR3(worldMtx._41, worldMtx._42, worldMtx._43);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getPosition()
{
	return pos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 回転取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getRotation()
{
	return rot;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// あたり判定用半径取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
FLOAT Pawn::getCollisionRadius()
{
	switch (meshType)
	{
	case MeshObjType::NormalModel:
		return meshData.collisionRadus;
	case MeshObjType::HierarchyModel:
		return hierarchymeshData.collisionRadus;
	default:
		break;
	}

	return meshData.collisionRadus;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// あたり判定用中心座標取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getCenterPos()
{
	return hierarchymeshData.centerPos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// あたり判定用位置取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getColliderPos()
{
	return colliderPtr->figurePtr->GetPos();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ情報セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setMesh(LPD3DXMESH Mesh, LPD3DXBUFFER mat, DWORD NumMat)
{
	pD3DXMesh	 = Mesh;
	pD3DXBuffMat = mat;
	uNumMat		 = NumMat;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPD3DXMESH Pawn::getMesh()
{
	return meshData.pD3DXMesh;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// マテリアル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPD3DXBUFFER Pawn::getMat()
{
	return pD3DXBuffMat;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// マテリアル数取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
DWORD Pawn::getMatNum()
{
	return uNumMat;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用中フラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setUsedFlg(bool setFlg)
{
	isUsed = setFlg;

	// コライダー使用フラグセット
	if(colliderPtr)
		colliderPtr->SetUsedFlg(setFlg);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 移動量セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setMoveSpeed(D3DXVECTOR3 Move)
{
	move = Move;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 目的着地位置セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setDestLanding(D3DXVECTOR3 SetLanding)
{
	destLanding = SetLanding;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 当たったオブジェクトのインデックスをセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setHitIndex(INT nSetIndex)
{
	hitIndex = nSetIndex;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setNumber(UINT uSetNumber)
{
	idNumber = uSetNumber;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 移動スピード取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getMove()
{
	return move;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPDIRECT3DVERTEXBUFFER9 Pawn::getVtxBuff()
{
	return pD3DVtxBuffPawn;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPDIRECT3DTEXTURE9 Pawn::getTexture()
{
	return pD3DTexture;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用フラグ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BOOL Pawn::getUsedFlg()
{
	return isUsed;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 当たり判定ボックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Pawn::getCollisionBox()
{
	switch (meshType)
	{
	case MeshObjType::NormalModel:
		return meshData.collitionBox;
	case MeshObjType::HierarchyModel:
		return hierarchymeshData.collitionBox;
	default:
		break;
	}
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タグ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Pawn::TagType Pawn::getTag()
{
	return tagName;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// あたり判定フラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setHitFlg(bool bSet)
{
	isHit = bSet;
}
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// インデックスバッファアクセス用ポインタ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
WORD* Pawn::getIndxAcess()
{
//	return pIndx;
	return meshData.pIndx;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点アクセス用ポインタ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
MESH_VTX* Pawn::getVtxAcess()
{
	//return pVtx;
	return meshData.pVtx;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点の数取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
DWORD Pawn::getVertexNum()
{
	return meshData.dwNumVtx;

//	return dwNumVtx;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// インデックスバッファの数取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
DWORD Pawn::getIndxNum()
{
//	return dwNumIndx;
	return meshData.dwNumIndx;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 三角形の数取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
DWORD Pawn::getTriangleNum()
{
//	return dwNumTriangles;
	return meshData.dwNumTriangles;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Rayと三角形のあたり判定用の値セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setRayToTriangleInfo(D3DXVECTOR3 SetCross, D3DXVECTOR3 SetNormal)
{
	cross  = SetCross;
	normal = SetNormal;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//  階層構造用 境界球/境界ボックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::calcCollision(LPD3DXFRAME pFrame)
{
	D3DXVECTOR3& vMax = collitionBox;
	D3DXVECTOR3& vMin = centerPos;
	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	collisionRadus = -1.0f;
	calcCollisionFrame(pFrame);
	D3DXVECTOR3 vBBox, vCenter;
	vBBox	= (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	collitionBox	= vBBox;
	centerPos		= vCenter;
	collisionRadus = 0.0f;
	calcCollisionFrame(pFrame);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレーム毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::calcCollisionFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) 
	{
		// メッシュコンテナ毎の頂点座標の抽出
		calcCollisionMeshContainer(pMeshContainer, pFrame);
		// 次のメッシュコンテナ
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	// 兄弟フレームがあれば兄弟フレームを描画
	if (pFrame->pFrameSibling)
		calcCollisionFrame(pFrame->pFrameSibling);
	// 子フレームがあれば子フレームを描画
	if (pFrame->pFrameFirstChild)
		calcCollisionFrame(pFrame->pFrameFirstChild);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュコンテナ毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::calcCollisionMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->combinedTransformationMatrix;
	// 頂点座標の抽出
	LPD3DXMESH pMesh = pMeshContainer->MeshData.pMesh;
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVtx = pMesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (collisionRadus < 0.0f) 
		{
			D3DXVECTOR3& vMax = collitionBox;
			D3DXVECTOR3& vMin = centerPos;
			if (vMax.x < vtx.x) vMax.x = vtx.x;
			if (vMax.y < vtx.y) vMax.y = vtx.y;
			if (vMax.z < vtx.z) vMax.z = vtx.z;
			if (vMin.x > vtx.x) vMin.x = vtx.x;
			if (vMin.y > vtx.y) vMin.y = vtx.y;
			if (vMin.z > vtx.z) vMin.z = vtx.z;
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - centerPos));
			if (collisionRadus < fRadius)
				collisionRadus = fRadius;
		}
	}
	pMesh->UnlockVertexBuffer();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 書き出し用データ更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::updateExportData()
{
	exportData.PosData = pos;		// 位置
	exportData.RotData = rot;		// 回転
	exportData.ScaleData = scale;	// 拡大率
	exportData.ObjType = objType;		// オブジェクトの種類
	exportData.uNumber = idNumber;		// 識別番号
	exportData.bUsed = isUsed;		// 使用フラグ
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 書き出し用データセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setExportData(EXPORT_GAMEOBJ_DATA SetData)
{
	pos = SetData.PosData;		// 位置
	rot = SetData.RotData;		// 回転
	scale = SetData.ScaleData;		// 拡大率
	objType = SetData.ObjType;		// ブロックの種類
	idNumber = SetData.uNumber;		// 識別番号
	isUsed = SetData.bUsed;			// 使用フラグ
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 書き出し用データ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
EXPORT_GAMEOBJ_DATA* Pawn::getExportData()
{
	return &exportData;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期値をセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setDefaultValue()
{
	defPos			= pos;
	defMove			= move;
	defrot			= rot;
	defScale		= scale;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期値にセットしなおし
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::initializeStatus()
{
	pos		= defPos;
	move	= defMove;
	rot		= defrot;
	scale	= defScale;

	accele = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// 回転
	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// 移動
	D3DXMATRIX translateMtx;
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);



}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 地面上フラグ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Pawn::getIsGround()
{
	return isGround;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 地面上フラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setIsGround(bool bSet)
{
	isGround = bSet;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメーション切り替え
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Pawn::setAnimChange(UINT uSetNum,UINT uWorkNum)
{
	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	hierarchymeshData.animCtrl->GetTrackDesc(currentAnim, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	hierarchymeshData.animCtrl->SetTrackAnimationSet(uWorkNum, hierarchymeshData.ppAnimset[currentAnim]);
	hierarchymeshData.animCtrl->SetTrackDesc(uWorkNum, &TD);

	// 新しいアニメーションセットをトラック0に設定
	hierarchymeshData.animCtrl->SetTrackAnimationSet(0, hierarchymeshData.ppAnimset[uSetNum]);

	currentAnim = uSetNum;
}