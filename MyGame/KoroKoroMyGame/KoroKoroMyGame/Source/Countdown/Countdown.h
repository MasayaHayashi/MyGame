//������������������������������������������������
// C_LoadUiIcon.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X��` =====
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