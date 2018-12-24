//������������������������������������������������
// Fream.h
// �^�C�g���p�I�u�W�F�N�g�N���X
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef FREAM_H
#define FREAM_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
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