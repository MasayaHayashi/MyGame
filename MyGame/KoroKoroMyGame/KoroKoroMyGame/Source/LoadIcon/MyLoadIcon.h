//������������������������������������������������
// C_LoadUiIcon.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef LOAD_ICON_H
#define LOAD_ICON_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X��` =====
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