//��������������������������������������������������������������
// SceneTitle.h
// Author	 : Masaya Hayashi
//��������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_TITLE
#define SCENE_TITLE

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"
#include "../../Skydome/Skydome.h"

// ===== �񋓑̒�` =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== �N���X�̑O���錾 =====
class C_MAIN_FIELD;
class Player;
class TitleUI;
class C_PARTICLE_BASE;
class HeartObj;
class Light;
class Camera;
class FadeUI;
class Board;

// ===== �N���X��` =====
class SceneTitle final : public C_SCENE_BASE
{
public:
	SceneTitle();
	~SceneTitle();

	void initialize();
	void update();
	void draw();
	void finalize();

	void initializeStatus();

	Camera* getCamera();

private:
	static constexpr UINT MaxUIType = static_cast<UINT>(UIType::DescUI) + 1;

	Camera					*pCamera;							// �J����
	Light					*pLight;							// ���C�g

	Skydome					*pSkydome;							// �X�J�C�h�[��


	Player				*pPlayer;
	C_MAIN_FIELD			*pField;
	TitleUI					*pTitleUI;							// �^�C�g��UI
	HeartObj				*pTitleObj;							// �^�C�g���I�u�W�F�N�g
	Board					*pBoard[MaxUIType];					// UI�֘A
//	C_PARTICLE_BASE			*pParticle[MAX_PARTICLE];			// �p�[�e�B�N��

	bool					bChangeScene;						// �V�[���؂�ւ�
	INT						nChangeSceneWaitCnt;				// �ҋ@�p�J�E���^
	UINT					uSelectScene;

};

#endif