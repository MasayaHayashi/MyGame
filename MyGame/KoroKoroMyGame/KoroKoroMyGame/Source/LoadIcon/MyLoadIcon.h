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
class MyLoadIcon final : public Board
{
public:
	MyLoadIcon();
	~MyLoadIcon();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	const std::string TexturePass = "Data/Texture/Loadeffect.png";
	
	static constexpr size_t FadeFream = 100;
	INT fadeCnt = FadeFream;

};

#endif