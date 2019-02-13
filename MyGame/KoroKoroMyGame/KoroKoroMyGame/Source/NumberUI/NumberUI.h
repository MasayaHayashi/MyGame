//������������������������������������������������
// NumberUI.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef NUMBER_UI_H
#define NUMBER_UI_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X��` =====
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