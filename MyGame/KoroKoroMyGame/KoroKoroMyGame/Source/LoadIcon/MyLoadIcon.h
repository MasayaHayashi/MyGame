//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_LoadUiIcon.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef LOAD_ICON_H
#define LOAD_ICON_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラス定義 =====
class MyLoadIcon : public Board
{
public:
	MyLoadIcon();
	~MyLoadIcon();

	void initialize();
	void finalize();
	void update();
	void draw();

private:

};

#endif