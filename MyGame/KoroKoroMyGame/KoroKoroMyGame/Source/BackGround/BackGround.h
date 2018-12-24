//������������������������������������������������
// BackGround.h
// Author : MasayaHayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef BACK_GROUND_H
#define BACK_GROUND_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
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