//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// HeartObj.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef HEART_OBJ_H
#define HEART_OBJ_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"

// ===== 定数・マクロ定義 =====

// ===== クラスの前方宣言 =====
class C_COLLIDER;

// ===== 構造体定義 =====

// ===== 列挙体定義 =====

// ===== クラス定義 =====
class HeartObj final : public Pawn
{
public:
	HeartObj();
	~HeartObj();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj);

private:

	const std::string ModelFilePass		= "Data/Model/Heart/heart.x";
	const std::string TextureFilePass	= "Data/Model/Heart/heart.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	

	FLOAT rotCnt = 0;

	void initializeTitleObj_Title();
	void initializeTitleObj_GameMain();
	void initializeTitleObj_Result();
	void UpdateTitleObj_Title();
	void UpdateTitleObj_GameMain();
	void UpdateTitleObj_Result();


	SceneManager::SceneState currentScene;
	C_COLLIDER *pCollider;
};

#endif 
