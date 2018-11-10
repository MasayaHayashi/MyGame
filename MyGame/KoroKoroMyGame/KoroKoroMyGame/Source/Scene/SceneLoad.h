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

// ===== �N���X�̑O���錾 =====
class C_LOAD_UI_ICON;
class Light;
class C_CAMERA;
class HeartObj;
class FadeUI;

// ===== �񋓑̒�` =====
enum class LoadState
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

	void initialize();
	void finalize();
	void EnableLoad();
	void updateLoad();
	void drawScene();

	// �Z�b�^�[�֘A

	// �ÓI�����o�֐�
	static void setLoadFlg(bool);

private:
	std::mutex mutex;

	C_CAMERA		*pCamera;
	Light			*pLight;
	HeartObj		*pTitleObj;
	FadeUI			*pFade;
	C_LOAD_UI_ICON	*pLoadIcon;

	// �ÓI�����o�ϐ�
	static bool	   finishedLoad;
};

#endif