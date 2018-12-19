//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// HeartObj.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef STAR_H
#define STAR_H

// ===== インクルード部 =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"

// ===== 定数・マクロ定義 =====

// ===== クラスの前方宣言 =====

// ===== 構造体定義 =====

// ===== 列挙体定義 =====

// ===== クラス定義 =====
class Star final : public Board
{
public:
	Star();
	~Star();

	void initialize();
	void finalize();
	void update();
	void draw();

private:

	const std::string ModelFilePass = "Data/Model/Heart/heart.x";
	const std::string TextureFilePass = "Data/Texture/GoldStar.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	void initializeTitleObj_Title();
	void initializeTitleObj_GameMain();
	void initializeTitleObj_Result();
	void UpdateTitleObj_Title();
	void UpdateTitleObj_GameMain();
	void UpdateTitleObj_Result();

	SceneManager::SceneState currentScene;
};

#endif 
