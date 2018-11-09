//��������������������������������������������������������������
// SceneTitle.h
// Author	 : Masaya Hayashi
//��������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_TITLE
#define SCENE_TITLE

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"

#include "C_Skydome.h"
#include "C_ParticleBase.h"

// ===== �񋓑̒�` =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== �N���X�̑O����` =====
class C_MAIN_FIELD;
class C_PLAYER;
class C_TITLE_UI;
class C_PARTICLE_BASE;
class C_TITLE_OBJ;
class C_CAMERA;
class C_LIGHT;
class C_FADE;

// ===== �N���X��` =====
class SceneTitle final : public C_SCENE_BASE
{
public:
	SceneTitle();
	~SceneTitle();

	void initialize();
	void update();
	const void draw();
	void finalize();

	void initStatus();

	C_CAMERA* getCamera();


private:
	C_CAMERA				*pCamera;							// �J����
	C_LIGHT					*pLight;							// ���C�g
	C_SKYDOME				*pSkydome;							// �X�J�C�h�[��



	C_PLAYER				*pPlayer;							// �v���C���[
	C_MAIN_FIELD			*pField;							// �t�B�[���h
	C_TITLE_UI				*pTitleUI;							// �^�C�g��UI
	C_TITLE_OBJ				*pTitleObj;							// �^�C�g���I�u�W�F�N�g
	C_BOARD					*pBoard[MAX_UI_TYPE];				// UI�֘A
	C_PARTICLE_BASE			*pParticle[MAX_PARTICLE];			// �p�[�e�B�N��

	bool					bChangeScene;						// �V�[���؂�ւ�
	INT						nChangeSceneWaitCnt;				// �ҋ@�p�J�E���^
	UINT					uSelectScene;

};

#endif