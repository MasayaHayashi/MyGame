//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_ResourceManager.h
// リソース管理(メッシュ、テクスチャ管理)
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// ===== インクルード部 =====
#include "d3dx9.h"
#include "../Mesh/Mesh.h"
#include "../MyHierarchy/MyHierarchy.h"
#include <vector>

// ===== クラスの前方宣言 =====

// ===== 構造体定義 =====
typedef struct
{
	CHAR				szMeshFileName[256];	// ファイル名
	LPD3DXMESH			pD3DXMesh;				// メッシュ情報
	LPD3DXBUFFER		pD3DXBuffMat;			// マテリアル情報へのポインタ
	DWORD				numMat;					// マテリアル情報の数
	D3DXVECTOR3			maxVtx;					// 最大頂点位置
	D3DXVECTOR3			minVtx;					// 最小頂点位置
	D3DXVECTOR3			centerPos;				// モデルの中心座標
	D3DXVECTOR3			collitionBox;			// あたり判定用サイズ
	FLOAT				collisionRadus;			// あたり判定用サイズ

	DWORD				dwNumVtx;				// 頂点数
	DWORD				dwNumTriangles;			// 三角形の数 (頂点 * 3)
	DWORD				dwNumIndx;				// インデックスバッファの数
	DWORD				dwAttrNum;				// 属性値
	D3DXATTRIBUTERANGE*	pAttr;					// 属性値

	MESH_VTX			*pVtx;					// 頂点情報へのアクセス用ポインタ
	WORD				*pIndx;					// インデックスバッファアクセス用ポインタ
	
} MESH_DATA;	// メッシュ情報

typedef struct
{
	CHAR						meshFileName[256];		// ファイル名
	LPD3DXMESH					pD3DXMesh;				// メッシュ情報
	LPD3DXBUFFER				pD3DXBuffMat;			// マテリアル情報へのポインタ
	DWORD						numMat;					// マテリアル情報の数
	D3DXVECTOR3					maxVtx;					// 最大頂点位置
	D3DXVECTOR3					minVtx;					// 最小頂点位置
	D3DXVECTOR3					centerPos;				// モデルの中心座標
	D3DXVECTOR3					collitionBox;			// あたり判定用サイズ
	FLOAT						collisionRadus;			// あたり判定用サイズ

	LPD3DXFRAME					frameRoot;				// ルート フレーム オブジェクト
	LPD3DXANIMATIONCONTROLLER	animCtrl;				// アニメーション コントローラ オブジェクト
	UINT						numAnimSet;				// アニメーション セット数
	LPD3DXANIMATIONSET*			ppAnimSet;				// アニメーション セット
	MyHierarchy					hierarchy;				// 階層メモリ確保/解放クラス
	DWORD						dwPrev;					// 直前の時刻
} HIERARCHY_MESH_DATA;	// 階層構造用メッシュ情報

typedef struct
{
	CHAR				texFileName[256];		// 読み込むファイル名
	LPDIRECT3DTEXTURE9	pD3DTexture;			// テクスチャへのポインタ
} TEXTURE_DATA;	// テクスチャ情報

typedef struct
{
	CHAR					name[256];				// 識別名
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffBoard;		// 頂点バッファインターフェースへのポインタ
	UINT					boardType;				// ボードの種類
	D3DXVECTOR3				posBoard;				// 位置
	D3DXVECTOR3				sizeBoard;				// 大きさ
	D3DXVECTOR3				scaleBoard;				// 拡大率
	D3DXVECTOR3				rotBoard;				// 回転角度
	FLOAT					radAngleBoard;			// 2D用回転角度
	bool					fade;					// フェード用
} VERTEX_BOARD_DATA;

// ===== クラス定義 =====
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// 生成
	static HRESULT MakeModel(MESH_DATA &MeshData, CHAR *pszFilename,UINT &uMeshType);
	static HRESULT CreateTexture(TEXTURE_DATA &TextureData,CHAR *pszFilename);
	static HRESULT MakeModelHierarchy(HIERARCHY_MESH_DATA &HierarchyMedhData, CHAR *pszFilename, UINT &MeshType);
	static HRESULT MakeVertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszFilename);

	// 解放
	static bool DestroyMesh(CHAR *pszChakNeme);
	static bool DestroyAllMesh();	// 全メッシュを削除
	static bool DestroyHierarchyMesh(CHAR *pszChakNeme);
	static bool DestroyAllHierarchyMesh();
	static bool DestroyAllTexture();
	static bool DestroyVtx();
	static bool DestroyFadeVtx();
	
	static void CreateFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename);

	// インスタンス
	static C_RESOURCE_MANAGER *pInstance;

private:
#define FVF_TVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

	C_RESOURCE_MANAGER();

	static std::vector<MESH_DATA*>				mesh;			
	static std::vector<HIERARCHY_MESH_DATA*>	hierarchyMesh;
	static std::vector<TEXTURE_DATA*>			texture;		
	static std::vector<TEXTURE_DATA*>			fadeTexture;	
	static std::vector<VERTEX_BOARD_DATA*>		vtxBoard;		
	static std::vector<VERTEX_BOARD_DATA*>		vtxFadeBoard;

	static bool CheckExisting(CHAR *pszChakNeme, MESH_DATA &meshData);
	static bool CheckExisting(CHAR *pszChakNeme, TEXTURE_DATA &textureData);
	static bool CheckExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &textureData);

static void SetTime(DOUBLE dTime, CHAR *pszFilename);

static 	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename);
static 	HRESULT AllocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename);
static	void CalcCollision(LPD3DXFRAME pFrame, CHAR *pszFilename);
static	void CalcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename);
static	void CalcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename);
static	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

static 	bool makeVtx(VERTEX_BOARD_DATA&);		// 通常
static 	bool makeVtxFade(VERTEX_BOARD_DATA&);	// フェード用

static	void createNormalTexture(TEXTURE_DATA&,CHAR*);

};


#endif