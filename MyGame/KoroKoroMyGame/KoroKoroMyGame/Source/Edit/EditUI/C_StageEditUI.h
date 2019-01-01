//������������������������������������������������
// C_StageEditUI.h
// �X�e�[�W�G�f�B�b�g�\�L
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef __C_STAGE_EDIT_UI_H__
#define __C_STAGE_EDIT_UI_H__

// ===== �C���N���[�h�� =====
#include "C_Board.h"
#include "C_SceneManager.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
class C_STAGE_EDIT_UI : public C_BOARD
{
public:
	C_STAGE_EDIT_UI();		// �R���X�g���N�^
	~C_STAGE_EDIT_UI();		// �f�X�g���N�^

	virtual void InitObject();
	virtual void UninitObject();
	virtual void UpdateObject();
	virtual void DrawObject();

private:

	C_SCENE_MANAGER::SCENE_STATE CurrentState;

};

#endif