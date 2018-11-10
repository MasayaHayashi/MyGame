//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MyHierarchy.cpp
// アニメーションメッシュ用 階層構造メモリ確保/解放 クラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "MyHierarchy.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p!=nullptr) { delete (p);     (p) = nullptr; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p!=nullptr) { (p)->Release(); (p) = nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p!=nullptr) { delete[] (p);   (p) = nullptr; } }
#endif
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレーム生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT MyHierarchy::CreateFrame(LPCSTR pMesh, LPD3DXFRAME* ppNewFrame)
{
	*ppNewFrame = nullptr;

	// フレーム用メモリ確保
	MYFRAME* pFrame = new MYFRAME;
	if (pFrame == nullptr) 
		return E_OUTOFMEMORY;

	ZeroMemory(pFrame, sizeof(MYFRAME));

	// フレーム名のコピー
	if (pMesh)
	{
		// フレーム名あり
		pFrame->Name = new char[lstrlenA(pMesh) + 1];
		if (pFrame->Name == nullptr)
		{
			delete pFrame;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pFrame->Name, pMesh);
	}
	else
	{
		// フレーム名なし
		pFrame->Name = nullptr;
	}

	// マトリックス初期化
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->combinedTransformationMatrix);

	// 確保したフレームを返す
	*ppNewFrame = pFrame;
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュコンテナ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT MyHierarchy::CreateMeshContainer(LPCSTR					name,
	CONST D3DXMESHDATA*		meshDataPtr,
	CONST D3DXMATERIAL*		materialsPtr,
	CONST D3DXEFFECTINSTANCE*	effectInstancesPtr,
	DWORD						materials,
	CONST DWORD*				adjacencyPtr,
	LPD3DXSKININFO				skinInfoPtr,
	LPD3DXMESHCONTAINER*		ppNewMeshContainer)
{
	// 取りあえずnullptrをセットしておく
	*ppNewMeshContainer = nullptr;

	// 正常なメッシュ以外はエラーで戻す
	if (meshDataPtr->Type != D3DXMESHTYPE_MESH || meshDataPtr->pMesh->GetFVF() == 0)
	{
		return E_FAIL;
	}

	// メッシュオブジェクト取得
	LPD3DXMESH meshPtr = meshDataPtr->pMesh;

	// メッシュコンテナ用メモリの確保
	MYMESHCONTAINER* meshContainerPtr = new MYMESHCONTAINER();

	if (meshContainerPtr == nullptr)
		return E_OUTOFMEMORY;

	ZeroMemory(meshContainerPtr, sizeof(MYMESHCONTAINER));

	// メッシュ名のコピー
	if (name)
	{
		meshContainerPtr->Name = new CHAR[lstrlenA(name) + 1];
		if (meshContainerPtr->Name == nullptr)
		{
			delete meshContainerPtr;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(meshContainerPtr->Name, name);
	}
	else
		meshContainerPtr->Name = nullptr;

	// D3Dデバイス取得
	LPDIRECT3DDEVICE9 devicePtr = nullptr;
	meshPtr->GetDevice(&devicePtr);

	// ポリゴン(三角形)数取得
	INT facesAmount = meshPtr->GetNumFaces();

	// 法線有無のチェック
	if (meshPtr->GetFVF() & D3DFVF_NORMAL)
	{
		meshContainerPtr->MeshData.pMesh = meshPtr;
		meshPtr->AddRef();
	}
	else
	{
		// 法線を持たない場合は追加
		if (FAILED(meshPtr->CloneMeshFVF(meshPtr->GetOptions(), meshPtr->GetFVF() | D3DFVF_NORMAL, devicePtr, &meshContainerPtr->MeshData.pMesh)))
		{
			devicePtr->Release();
			DestroyMeshContainer(meshContainerPtr);
			return E_FAIL;
		}
		meshPtr = meshContainerPtr->MeshData.pMesh;
		D3DXComputeNormals(meshPtr, nullptr);
	}

	// マテリアル用メモリ確保
	meshContainerPtr->NumMaterials = max(1, materials);
	meshContainerPtr->pMaterials = new D3DXMATERIAL[meshContainerPtr->NumMaterials];
	meshContainerPtr->ppTextures = new LPDIRECT3DTEXTURE9[meshContainerPtr->NumMaterials];
	meshContainerPtr->pAdjacency = new DWORD[facesAmount * 3];
	if (meshContainerPtr->pAdjacency == nullptr || meshContainerPtr->ppTextures == nullptr || meshContainerPtr->pMaterials == nullptr)
	{
		DestroyMeshContainer(meshContainerPtr);
		devicePtr->Release();
		return E_OUTOFMEMORY;
	}

	ZeroMemory(meshContainerPtr->ppTextures, sizeof(LPDIRECT3DTEXTURE9) * meshContainerPtr->NumMaterials);
	CopyMemory(meshContainerPtr->pAdjacency, adjacencyPtr, sizeof(DWORD) * facesAmount * 3);

	// マテリアルの設定
	if (materials > 0) 
	{
		// カレントディレクトリを変更
		TCHAR currentDir[_MAX_PATH];
		if (dir[0]) 
		{
			GetCurrentDirectory(_MAX_PATH, currentDir);
			SetCurrentDirectory(dir);
		}

		CopyMemory(meshContainerPtr->pMaterials, materialsPtr, sizeof(D3DXMATERIAL) * materials);
		for (DWORD i = 0; i < materials; ++i) 
		{
			meshContainerPtr->pMaterials[i].MatD3D.Ambient = meshContainerPtr->pMaterials[i].MatD3D.Diffuse;
			if (FAILED(D3DXCreateTextureFromFileA(devicePtr, meshContainerPtr->pMaterials[i].pTextureFilename, &meshContainerPtr->ppTextures[i])))
				meshContainerPtr->ppTextures[i] = nullptr;
		}

		// カレントディレクトリを元に戻す
		if (dir[0])
			SetCurrentDirectory(currentDir);
	}
	else 
	{
		ZeroMemory(meshContainerPtr->pMaterials, sizeof(D3DXMATERIAL));
		meshContainerPtr->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		meshContainerPtr->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		meshContainerPtr->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		meshContainerPtr->pMaterials[0].MatD3D.Ambient = meshContainerPtr->pMaterials[0].MatD3D.Diffuse;
	}

	// 属性テーブルを取得
	HRESULT hr = meshContainerPtr->MeshData.pMesh->CloneMeshFVF(D3DXMESH_MANAGED, meshContainerPtr->MeshData.pMesh->GetFVF(), devicePtr, &meshPtr);
	if (FAILED(hr))
	{
		devicePtr->Release();
		return hr;
	}
	SAFE_RELEASE(meshContainerPtr->MeshData.pMesh);
	meshContainerPtr->MeshData.pMesh = meshPtr;
	hr = meshPtr->GetAttributeTable(nullptr, &meshContainerPtr->NumAttributeGroups);
	if (FAILED(hr)) 
	{
		devicePtr->Release();
		return hr;
	}
	meshContainerPtr->pAttributeTable = new D3DXATTRIBUTERANGE[meshContainerPtr->NumAttributeGroups];
	if (meshContainerPtr->pAttributeTable == nullptr)
	{
		meshContainerPtr->NumAttributeGroups = 0;
		devicePtr->Release();
		return E_OUTOFMEMORY;
	}
	hr = meshPtr->GetAttributeTable(meshContainerPtr->pAttributeTable, nullptr);
	if (FAILED(hr))
	{
		devicePtr->Release();
		return hr;
	}

	// D3Dデバイス解放
	devicePtr->Release();

	// スキン情報があるか
	if (skinInfoPtr)
	{
		// スキン情報を保存(参照カウンタ加算)
		meshContainerPtr->pSkinInfo = skinInfoPtr;
		skinInfoPtr->AddRef();

		// ワールド・マトリックスの場所を確保
		DWORD dwBoneNum = skinInfoPtr->GetNumBones();
		meshContainerPtr->pBoneOffsetMatrix = new D3DXMATRIX[dwBoneNum];
		if (meshContainerPtr->pBoneOffsetMatrix == nullptr)
			return E_OUTOFMEMORY;

		// ワールド・マトリックスを初期化
		for (DWORD i = 0; i < dwBoneNum; ++i)
			meshContainerPtr->pBoneOffsetMatrix[i] = *skinInfoPtr->GetBoneOffsetMatrix(i);

		// スキン用に変換
		hr = skinInfoPtr->ConvertToBlendedMesh(
			meshPtr, 0, meshContainerPtr->pAdjacency,
			nullptr, nullptr, nullptr,
			&meshContainerPtr->dwWeight,
			&meshContainerPtr->dwBone,
			&meshContainerPtr->pBoneBuffer,
			&meshContainerPtr->MeshData.pMesh);

		if (FAILED(hr))
			return hr;
	}

	// 確保したメッシュコンテナを返す
	*ppNewMeshContainer = meshContainerPtr;
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレーム破棄
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT MyHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameToFree;
	SAFE_DELETE_ARRAY(pFrame->Name);
	SAFE_DELETE(pFrame);
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュコンテナ破棄
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT MyHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER meshContainerPtrToFree)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrToFree;

	SAFE_DELETE_ARRAY(meshContainerPtr->Name);
	SAFE_DELETE_ARRAY(meshContainerPtr->pAdjacency);
	SAFE_DELETE_ARRAY(meshContainerPtr->pMaterials);

	// 全テクスチャの解放
	if (meshContainerPtr->ppTextures) 
	{
		for (DWORD i = 0; i < meshContainerPtr->NumMaterials; i++) 
			SAFE_RELEASE(meshContainerPtr->ppTextures[i]);
	}

	SAFE_DELETE_ARRAY(meshContainerPtr->ppTextures);

	SAFE_RELEASE(meshContainerPtr->MeshData.pMesh);

	SAFE_DELETE_ARRAY(meshContainerPtr->pAttributeTable);
	meshContainerPtr->NumAttributeGroups = 0;

	SAFE_RELEASE(meshContainerPtr->pBoneBuffer);
	SAFE_DELETE_ARRAY(meshContainerPtr->pBoneOffsetMatrix);
	SAFE_DELETE_ARRAY(meshContainerPtr->ppBoneMatrix);

	SAFE_DELETE(meshContainerPtr);
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
MyHierarchy::MyHierarchy()
{
	dir[0] = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Xファイルのあるディレクトリを設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void MyHierarchy::setDirectory(LPCTSTR pszDir)
{
	if (pszDir)
		lstrcpy(dir, pszDir);
	else
		dir[0] = 0;
}