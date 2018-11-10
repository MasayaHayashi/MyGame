//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_MainField.h
// メインフィールドクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MAINFIELD_H
#define MAINFIELD_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"

// ===== 定数・マクロ定義 =====
#define FIELD_SIZE (5300.0f)	//  2/22 5300
#define FIELD_RADIUS (FIELD_SIZE * 0.5f)
#define HIGH_POS (50.0f)	// 最大高度
#define LOW_POS (0.0f)		// 着地位置

// ===== クラス定義 =====
class C_MAIN_FIELD : public Pawn
{
public:
	C_MAIN_FIELD();
	~C_MAIN_FIELD();
	virtual	void initializeMeshField();
	virtual	void UninitMeshField();
	virtual	void UpdateMeshField();
	virtual	void DrawMeshField();
	D3DXVECTOR3 GetCenterPos();		// 中心座標取得
	void SetMaterialcolorEmissive(float fRed, float Green, float Blue, float Alpha);
	bool GetCollisionCiecle(D3DXVECTOR3 CurrentPos, float);
	FLOAT getCollisionRadius();

private:
};

#endif
