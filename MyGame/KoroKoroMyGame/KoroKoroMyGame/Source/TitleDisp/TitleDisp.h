//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// TitleDisp.h
// タイトル用説明
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef TITLE_DISP_H
#define TITLE_DISP_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class TitleDisp final : public Board
{
public:
	TitleDisp();
	~TitleDisp();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	const std::string TextureFilePass = "Data/Texture/TitleSetsumei.png";
	const FLOAT MoveSpeed = 1.5f;
};

#endif