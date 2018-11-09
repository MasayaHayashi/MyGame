//������������������������������������������������
// C_SceneStageEdit.h
// �V�[���^�C�g���N���X
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef __C_SCENE_STAGE_EDIT_H__
#define __C_SCENE_STAGE_EDIT_H__

// ===== �C���N���[�h�� =====
#include "C_SceneBase.h"
#include "C_Block.h"

// ===== �萔�E�}�N����` =====

// ===== �񋓑̒�` =====

// ===== �N���X�̑O����` =====
class C_LIGHT;
class C_CAMERA;
class C_SKYDOME;
class C_STAGE_EDITOR;

// ===== �N���X��` =====
class C_SCENE_STAGE_EDIT : public C_SCENE_BASE
{
public:
	C_SCENE_STAGE_EDIT();	// �R���X�g���N�^
	~C_SCENE_STAGE_EDIT();	// �f�X�g���N�^

	void InitScene();
	void UninitScene();
	void UpdateScene();
	void DrawScene();

	void InitStatus();

	// �Q�b�^�[�֘A
	C_CAMERA* GetCamera();


private:

	C_SKYDOME				*pSkydome;									// �X�J�C�h�[��
	C_STAGE_EDITOR			*pStageEditor;								// �X�e�[�W�G�f�B�^�[

};

#endif