//������������������������������������������������
// C_TitleUI.h
// �^�C�g���p�I�u�W�F�N�g�N���X
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef TITLE_UI_H
#define TITLE_UI_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
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