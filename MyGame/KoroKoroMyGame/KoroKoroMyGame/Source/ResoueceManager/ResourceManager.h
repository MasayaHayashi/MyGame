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
#include <unordered_map>

// ===== 前方宣言 =====
enum class boardType;
enum class MeshObjType;

// ===== 構造体定義 =====
typedef struct
{
	CHAR				meshFileName[256];	// ファイル名
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
	LPD3DXANIMATIONCONTROLLER	animCtrlPtr;			// アニメーション コントローラ オブジェクト
	UINT						numAnimset;				// アニメーション セット数
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
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// 頂点バッファインターフェースへのポインタ
	boardType				boardType;				// ボードの種類
	D3DXVECTOR3				pos;					// 位置
	D3DXVECTOR3				size;					// 大きさ
	D3DXVECTOR3				scale;				// 拡大率
	D3DXVECTOR3				rot;				// 回転角度
	FLOAT					radAngle;			// 2D用回転角度
	bool					fade;					// フェード用
} VERTEX_BOARD_DATA;

// ===== クラス定義 =====
class ResourceManager
{
public:
	// 生成
	static HRESULT makeModel(MESH_DATA &MeshData, CHAR *pszFilename, MeshObjType &uMeshType);
	static HRESULT createTexture(TEXTURE_DATA &TextureData,CHAR *pszFilename);
	static HRESULT makeModelHierarchy(HIERARCHY_MESH_DATA &HierarchyMedhData, CHAR *pszFilename, std::string keyName, MeshObjType &MeshType);
	static HRESULT makevertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszFilename);

	// 解放
	static bool destroyMesh(CHAR *pszChakNeme);
	static bool destroyAllMesh();	// 全メッシュを削除
	static bool destroyHierarchymesh(CHAR *pszChakNeme,std::string keyName);
	static bool destroyAllHierarchymesh();
	static bool destroyAllTexture();
	static bool destroyVtx();
	static bool destroyFadeVtx();
	
	static void createFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename);

	static ResourceManager *pInstance;

private:
#define FVF_TVERTEX	    (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	ResourceManager();
	~ResourceManager();

	static std::vector<MESH_DATA*>				mesh;
	static std::unordered_map<std::string,HIERARCHY_MESH_DATA*>	hierarchyMesh;
	static std::vector<TEXTURE_DATA*>			texture;
	static std::vector<TEXTURE_DATA*>			fadeTexture;
	static std::vector<VERTEX_BOARD_DATA*>		vtxBoard;
	static std::vector<VERTEX_BOARD_DATA*>		vtxFadeBoard;

	static bool checkExisting(CHAR *pszChakNeme, MESH_DATA &meshData);
	static bool checkExisting(CHAR *pszChakNeme, TEXTURE_DATA &textureData);
	static bool checkExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &textureData);

	static void setTime(DOUBLE dTime, CHAR *pszFilename,std::string keyName);
	
	static 	HRESULT allocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename,std::string keyName);
	static 	HRESULT allocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename,std::string keyName);
	static	void calcCollision(LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName);
	static	void calcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName);
	static	void calcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName);
	static	void updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	
	static 	bool makeVtx(VERTEX_BOARD_DATA&);		// 通常
	static 	bool makeVtxFade(VERTEX_BOARD_DATA&);	// フェード用
	
	static	void createNormalTexture(TEXTURE_DATA&,CHAR*);

};


#endif