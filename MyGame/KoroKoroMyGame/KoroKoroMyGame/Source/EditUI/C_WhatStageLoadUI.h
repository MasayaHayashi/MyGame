//������������������������������������������������
// C_WhatStageLoadUI.h
// �ǂ̃X�e�[�W�ɂ��܂���
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef __C_WHAT_STAGE_LOAD_UI_H__
#define __C_WHAT_STAGE_LOAD_UI_H__

// ===== �C���N���[�h�� =====
#include "C_Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
class C_WHAT_STAGE_LOAD_UI : public C_BOARD
{
public:
	C_WHAT_STAGE_LOAD_UI();		// �R���X�g���N�^
	~C_WHAT_STAGE_LOAD_UI();		// �f�X�g���N�^

	void InitObject();
	void UninitObject();
	void UpdateObject();
	void DrawObject();

private:

};

#endif