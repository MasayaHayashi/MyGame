//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// BackGround.h
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef BACK_GROUND_H
#define BACK_GROUND_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class BackGround final : public Board
{
public:
	BackGround();
	~BackGround();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	std::string TexturePass = "Data/Texture/BackGround.jpg";

};

#endif