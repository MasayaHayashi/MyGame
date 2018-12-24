//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Fream.h
// タイトル用オブジェクトクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef FREAM_H
#define FREAM_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class Fream final : public Board
{
public:
	Fream();
	~Fream();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	std::string TexturePass = "Data/Texture/fream.png";

};

#endif