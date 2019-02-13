//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// NumberUI.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef NUMBER_UI_H
#define NUMBER_UI_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラス定義 =====
class NumberUI final : public Board
{
public:
	NumberUI();
	NumberUI(INT setNumber);
	NumberUI(INT setNumber, FLOAT scale);
	~NumberUI();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

	static constexpr INT MaxDigit = 3;
	bool changeNumber(UINT number);
	void changePosition(D3DXVECTOR3 setPos);
	void setSize(D3DXVECTOR3 size);

private:
	const std::string TexturePass = "Data/Texture/numbers.png";

	INT number = 0;


};

#endif