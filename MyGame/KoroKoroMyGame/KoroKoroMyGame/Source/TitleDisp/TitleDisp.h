//������������������������������������������������
// TitleDisp.h
// �^�C�g���p����
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef TITLE_DISP_H
#define TITLE_DISP_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
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