//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_LoadUiIcon.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラス定義 =====
class Countdown final : public Board
{
public:
	Countdown();
	~Countdown();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	const std::string TexturePass = "Data/Texture/numbers.png";
	static constexpr INT   ChangeFrame = 60;
	
	INT changeNumberCnt = ChangeFrame;

};

#endif