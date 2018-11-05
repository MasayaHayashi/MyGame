//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MyHierarchy.h
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MY_HIERARCHY_H
#define MY_HIERARCHY_H

// ===== インクルード部 =====
#include "d3dx9.h"

// ===== 構造体定義 =====

// フレームクラス
struct MYFRAME : public D3DXFRAME
{	// 最終的なマトリックスを格納するための変数を追加.
	D3DXMATRIX	combinedTransformationMatrix;
};

// メッシュコンテナ
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{	// テクスチャを保存するための変数を追加
	LPDIRECT3DTEXTURE9*		ppTextures;
	// 属性テーブル
	LPD3DXATTRIBUTERANGE	pAttributeTable;
	DWORD					NumAttributeGroups;
	// 以下はスキンメッシュ用
	DWORD			dwWeight;	// 重みの数
	DWORD			dwBone;		// ボーンの数
	LPD3DXBUFFER	pBoneBuffer;// ボーン・テーブル
	LPD3DXMATRIX*	ppBoneMatrix;// ボーンのワールド行列の先頭
	LPD3DXMATRIX	pBoneOffsetMatrix;// フレームとしてのボーンのワールド行列
};

// ===== クラス定義 =====
class MyHierarchy final : public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(
		LPCSTR name,
		CONST D3DXMESHDATA* meshDataPtr,
		CONST D3DXMATERIAL* materialsPtr,
		CONST D3DXEFFECTINSTANCE* effectInstancesPtr,
		DWORD materials,
		CONST DWORD* adjacencyPtr,
		LPD3DXSKININFO skinInfoPtr,
		LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(LPD3DXFRAME frameToFreePtr);
	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER meshContainerBasePtr);

	MyHierarchy();		// コンストラクタ
	void setDirectory(LPCTSTR pszDir);

private:
	TCHAR dir[_MAX_PATH];
};

#endif