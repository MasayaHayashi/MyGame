//������������������������������������������������
// C_SceneLoad.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef C_SCENE_LOAD_H
#define C_SCENE_LOAD_H

// ===== �C���N���[�h�� =====
#include "../SceneInterface/sceneBase.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O����` =====
class C_LOAD_UI_ICON;
class C_LIGHT;
class C_CAMERA;
class C_TITLE_OBJ;
class C_FADE;

// ===== �񋓑̒�` =====
enum LOAD_STATE
{
	LOADING = 0,
	LOAD_COMP,
	MAX_LOAD_STAGE,
};

// ===== �N���X��` =====
class C_SCENE_LOAD
{
public:
	C_SCENE_LOAD();
	~C_SCENE_LOAD();

	void InitScene();
	void UninitScene();
	void EnableLoad();
	void UpdateLoad();
	void DrawScene();

	// �Z�b�^�[�֘A

	// �ÓI�����o�֐�
	static void SetLoadFlg(bool);

private:
	C_CAMERA		*pCamera;
	C_LIGHT			*pLight;
	C_TITLE_OBJ		*pTitleObj;
	C_FADE			*pFade;
	C_LOAD_UI_ICON	*pLoadIcon;

	// �ÓI�����o�ϐ�
	static bool	   bFinishedLoad;					// ���[�h�I��
};

#endif