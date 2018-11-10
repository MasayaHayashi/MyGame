//������������������������������������������������
// C_SceneLoad.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_LOAD_H
#define SCENE_LOAD_H

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
class SceneLoad
{
public:
	SceneLoad();
	~SceneLoad();

	static void initialize();
	static void finalize();
	static void enable();
	static void update();
	static void draw();

	static void setLoadFlg(bool);

private:
	C_CAMERA		*pCamera;
	C_LIGHT			*pLight;
	C_TITLE_OBJ		*pTitleObj;
	C_FADE			*pFade;
	C_LOAD_UI_ICON	*pLoadIcon;

	// �ÓI�����o�ϐ�
	static bool	   finishedLoad;					// ���[�h�I��
};

#endif