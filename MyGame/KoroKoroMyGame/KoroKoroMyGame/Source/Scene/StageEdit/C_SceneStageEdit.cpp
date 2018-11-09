//����������������������������������������������
// C_SceneTitle.cpp
// �V�[���^�C�g���N���X
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "C_SceneStageEdit.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_MainField.h"
#include "C_Player.h"
#include "C_Enemy.h"
#include "C_Collision.h"
#include "C_StageEditor.h"
#include "C_Pause.h"
#include "C_Block.h"
#include "C_MoveBlock.h"
#include "C_ItemStar.h"
#include "C_GoalObj.h"
#include "C_Skydome.h"
#include "input.h"
#include "C_SceneManager.h"
#include <stdio.h>	// �t�@�C�����o�͗p

#if _DEBUG
	#include "debugproc.h"
#endif

//������������������������������������
// �R���X�g���N�^
//������������������������������������
C_SCENE_STAGE_EDIT::C_SCENE_STAGE_EDIT()
{

}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
C_SCENE_STAGE_EDIT::~C_SCENE_STAGE_EDIT()
{

}

//����������������������������������
// ������
//����������������������������������
void C_SCENE_STAGE_EDIT::InitScene()
{
	// �X�J�C�h�[��������
	pSkydome = NEW C_SKYDOME;
	pSkydome->InitObject();

	// �X�e�[�W�G�f�B�^�[��������
	pStageEditor = NEW C_STAGE_EDITOR;
	pStageEditor->InitObject();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void C_SCENE_STAGE_EDIT::UninitScene()
{
	// �X�J�C�h�[���㏈��
	pSkydome->UninitObject();
	SAFE_DELETE(pSkydome);

	// �X�e�[�W�G�f�B�^�[�㏈��
	pStageEditor->UninitObject();
	SAFE_DELETE(pStageEditor);
}

//������������������������������������
// �X�V
//������������������������������������
void C_SCENE_STAGE_EDIT::UpdateScene()
{
	// �X�J�C�h�[���X�V
	pSkydome->UpdateObject();

	// �X�e�[�W�G�f�B�^�[�X�V
	pStageEditor->UpdateObject();

	// �V�[���J��
	if (GetKeyboardTrigger(DIK_F1))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
	}
}

//��������������������������������������
// �`��
//��������������������������������������
void C_SCENE_STAGE_EDIT::DrawScene()
{
	// �X�J�C�h�[���`��
	pSkydome->DrawObject();

	// �X�e�[�W�G�f�B�^�[�`��
	pStageEditor->DrawObject();
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void C_SCENE_STAGE_EDIT::InitStatus()
{

}


//����������������������������������
// �J�����擾
//����������������������������������
C_CAMERA* C_SCENE_STAGE_EDIT::GetCamera()
{
	return pStageEditor->GetCamera();
}