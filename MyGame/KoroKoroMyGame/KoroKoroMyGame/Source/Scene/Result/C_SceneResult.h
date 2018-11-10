//������������������������������������������������
// C_SceneResult.h
// �V�[�����U���g�N���X
// Author : Ryota Inoue
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_RESULT
#define SCENE_RESULT

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"

// ===== �N���X�̑O����` =====
class C_SKYDOME;
class C_PLAYER;
class C_CAMERA;

// ===== �N���X��` =====
class C_SCENE_RESULT : public C_SCENE_BASE
{
public:
	C_SCENE_RESULT();
	~C_SCENE_RESULT();

	void InitScene();
	void UninitScene();
	void UpdateScene();
	void DrawScene();

	void InitStatus();

	// �Q�b�^�[�֘A
	C_CAMERA* GetCamera();

private:
	bool bSEPlay;			// ���Đ�����
	C_SKYDOME *pSkydome;	// �X�J�C�h�[��
	C_PLAYER  *pPlayer;		// �v���C���[

};

#endif