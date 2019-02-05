//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// StarUI.h
// UI用の星画像
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef STAR_UI_H
#define STAR_UI_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class StarUI final : public Board
{
public:
	StarUI();
	~StarUI();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	bool awake = false;

	const std::string TextureFilePass = "Data/Texture/GoldStar.png";
	const FLOAT MoveSpeed = 1.5f;
};

#endif