//������������������������������������������������
// C_SceneMain.h
// �V�[�����C���N���X
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"

/*
#include "C_Block.h"
#include "C_StarParticle.h"
#include "C_StageEditor.h"
#include "C_ScoreUi.h"
*/
// ===== �񋓑̒�` =====
enum PARTICLE_OBJ_TYPE
{
	OBJ_BILLBOARD_STAR_PARTICLE = 0,
	OBJ_2D_STAR_PARTICLE,

	MAX_PARTICLE_OBJ_TYPE,
};

enum UI_OBJ_TYPE
{
	OBJ_READY = 0,
	OBJ_MISS,
	OBJ_BG,
	OBJ_STAR,
	OBJ_TUTORIAL,
	OBJ_NEXT,
	MAX_UI_OBJ_TYPE,
};

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====


// ===== �N���X��` =====
class C_SCENE_MAIN : public C_SCENE_BASE
{
public:
	C_SCENE_MAIN();
	~C_SCENE_MAIN();

	void initialize();
	void finalize();
	void updateScene();
	void drawScene();

	void initializeStatus();

	// �Q�b�^�[�֘A
	Camera* getCamera();

private:

	void CheckUnProject(INT);
	void CheckCollision();
	void setScore();

	/*

	Light					*pLight;											  // ���C�g
	C_CAMERA				*pCamera;											  // �J����
	C_SKYDOME				*pSkydome;											  // �X�J�C�h�[��
	Player				*pPlayer;											  // �v���C���[
	C_COLLISION				*pCollision;										  // �Փ˗p
	C_PAUSE					*pPause;											  // �|�[�Y�p
	Pawn				*pGameObj[MAX_GAME_OBJ_TYPE][MAX_GAME_OBJ];			  // �Q�[���I�u�W�F�N�g
	C_PARTICLE_BASE 		*pParticleObj[MAX_PARTICLE_OBJ_TYPE][MAX_PARTICLE];   // �p�[�e�B�N���֘A
	Board					*pUIObj[MAX_UI_OBJ_TYPE];							  // UI�֘A
	C_SCORE_UI				*pUIScore[MAX_SCORE_DIGIT];							  // �X�R�A
	C_STAGE_LOADER			*pStageLoader;										  // �X�e�[�W���[�h
	C_GOAL					*pGoal;												  // �S�[���p
	C_HEART_BOARD			*pHeart;											  // �n�[�g
	
	INT						nRestartCnt;								// �ăX�^�[�g�J�E���g
	static UINT				uCurrentStage;								// ���݂̃X�e�[�W

	UINT					uIsHitBlock;								// �u���b�N�ɓ������Ă��邩

	CHAR					szStageFileName[MAX_STAGE][MAX_FILE_NAME];	// �X�e�[�W�ǂݍ��݂̃t�@�C����

	UINT					uPrevScore;									// 1�t���[���O�̃X�R�A
	
	*/
};