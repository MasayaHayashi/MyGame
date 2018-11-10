//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_TitleUI.h
// タイトル用オブジェクトクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef TITLE_UI_H
#define TITLE_UI_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class TitleUI final : public Board
{
public:
	TitleUI();
	~TitleUI();

	void initialize();
	void finalize();
	void update();
	void draw();

private:

};

#endif