//������������������������������������������������
// C_SceneLoad.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_LOAD_H
#define SCENE_LOAD_H

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O����` =====
class MyLoadIcon;
class C_TITLE_OBJ;
class C_FADE;

class Camera;
class Light;

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
	static void update();
	static void draw();

	static void enable();

	static void setLoadFlg(bool);

private:
	static std::unique_ptr<Camera>			cameraPtr;
	static std::unique_ptr<Light>			lightPtr;
	static std::unique_ptr<MyLoadIcon>		myLoadIConPtr;

	static bool	   isFinishedLoad;					// ���[�h�I��
};

#endif