//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_MainField.cpp
// メインフィールドクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 定数・マクロ定義 =====
#define FILE_NAME	 "data/MODEL/RockLayered_5.x"
#define TEX_FILENAME "data/TEXTURE/land.tga"

// ===== インクルード部 =====
#include "MainField.h"
#include <string>
#if _DEBUG
//#include "debugproc.h"
#endif

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_MAIN_FIELD::C_MAIN_FIELD()
{
#if 0
	strcpy_s(pawnFileName, FILE_NAME);
	strcpy_s(texFileName, TEX_FILENAME);
	pos = D3DXVECTOR3(0.0f, -3.0f, 1.0f);
	
	scale = D3DXVECTOR3(0.7f, 0.1f, 0.5f);

	pD3DXMeshPawn = nullptr;

	isUsed = true;
#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_MAIN_FIELD::~C_MAIN_FIELD()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フィールド初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MAIN_FIELD::initializeMeshField()
{
	// Xファイルの読み込み
	ResourceManager::makeModel(meshData, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フィールド後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MAIN_FIELD::UninitMeshField()
{
	// メッシュ解放
	ResourceManager::destroyAllMesh();

	// テクスチャ解放
	ResourceManager::destroyAllTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フィールド更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MAIN_FIELD::UpdateMeshField()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フィールド描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MAIN_FIELD::DrawMeshField()
{
	Pawn::drawObject();
//	ResourceManager *pResourceMgr = GetResourceManager();

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 中心座標取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 C_MAIN_FIELD::GetCenterPos()
{
	return centerPos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 円状のフィールドあたり判定取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool C_MAIN_FIELD::GetCollisionCiecle(D3DXVECTOR3 CurrentPos,float Radius)
{
	if ((CurrentPos.x - centerPos.x) * (CurrentPos.x - centerPos.x) + (CurrentPos.z - centerPos.z) * (CurrentPos.z - centerPos.z) <= ((collisionRadus - Radius ) * (collisionRadus - Radius ) ))
		return false;

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// マテリアルの色セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MAIN_FIELD::SetMaterialcolorEmissive(float fRed, float Green, float Blue, float Alpha)
{
	D3DXMATERIAL *pD3DXMat;

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMat->GetBufferPointer();

	pD3DXMat->MatD3D.Emissive.r = fRed;
	pD3DXMat->MatD3D.Emissive.g = Green;
	pD3DXMat->MatD3D.Emissive.b = Blue;
	pD3DXMat->MatD3D.Emissive.a = Alpha;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// あたり判定用サイズ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
float C_MAIN_FIELD::getCollisionRadius()
{
	return collisionRadus;
}