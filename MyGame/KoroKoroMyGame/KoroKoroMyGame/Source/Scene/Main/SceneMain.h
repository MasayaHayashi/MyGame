//������������������������������������������������
// C_SceneMain.h
// �V�[�����C���N���X
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"
#include "../../DirectX3D/DirectX3D.h"
#include <memory>
#include <list>

/*
#include "Block.h"
#include "C_StarStar.h"
#include "C_StageEditor.h"
#include "C_ScoreUi.h"
*/

// ===== �񋓑̒�` =====
enum Star_OBJ_TYPE
{
	OBJ_BILLBOARD_Star_Star = 0,
	OBJ_2D_Star_Star,

	MAX_Star_OBJ_TYPE,
};

enum UI_OBJ_TYPE
{
	OBJ_READY = 0,
	OBJ_MISS,
	OBJ_BG,
	OBJ_Star,
	OBJ_TUTORIAL,
	OBJ_NEXT,
	MAX_UI_OBJ_TYPE,
};

// ===== �N���X�̑O���錾 =====
class Light;
class Camera;
class Player;
class Pawn;
class Collision;
class BallObj;
class Board;

// ===== �N���X��` =====
class SceneMain : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

	Camera* getCamera();

private:

	enum class GameState
	{
		Ready = 0,
		Tutorial,
		Normal,
		Pause,
		Goal,
		Miss,
	};

	void checkUnProject(INT);
	void checkCollision();
	void setScore();

	static constexpr INT ReStartTime = 120;

	const std::string StageTutorialPass = "StageTutorial.bin";
	const std::string Stage1Pass		= "Stage1.bin";
	const std::string Stage2Pass		= "Stage2.bin";
	const std::string Stage3Pass		= "Stage3.bin";

	std::unique_ptr<Light>		lightPtr;
	std::unique_ptr<Camera>		cameraPtr;
	std::unique_ptr<Collision>	collisionPtr;

	std::list<BallObj*>  ballsPtr;
	std::list<std::unique_ptr<Player>>	 playeresPtr;
	std::list<std::unique_ptr<Pawn>>	 gameObjectesPtr;
	std::list<std::unique_ptr<Board>>	 boardObjectesPtr;

	GameState				currentGameState;
	static UINT				currentStage;
	static UINT				prevScore;


	/*
	Light					*pLight;											  // ���C�g
	C_CAMERA				*pCamera;											  // �J����
	C_SKYDOME				*pSkydome;											  // �X�J�C�h�[��
	Player				*pPlayer;											  // �v���C���[
	Collision				*pCollision;										  // �Փ˗p
	C_PAUSE					*pPause;											  // �|�[�Y�p
	Pawn				*pGameObj[MAX_GAME_OBJ_TYPE][MAX_GAME_OBJ];			  // �Q�[���I�u�W�F�N�g
	C_Star_BASE 		*pStarObj[MAX_Star_OBJ_TYPE][MAX_Star];   // �p�[�e�B�N���֘A
	Board					*pUIObj[MAX_UI_OBJ_TYPE];							  // UI�֘A
	C_SCORE_UI				*pUIScore[MAX_SCORE_DIGIT];							  // �X�R�A
	C_STAGE_LOADER			*pStageLoader;										  // �X�e�[�W���[�h
	C_GOAL					*pGoal;												  // �S�[���p
	C_HEART_BOARD			*pHeart;											  // �n�[�g
	
	INT						nReStartCnt;								// �ăX�^�[�g�J�E���g

	UINT					uIsHitBlock;								// �u���b�N�ɓ������Ă��邩

	CHAR					szStageFileName[MAX_STAGE][MAX_FILE_NAME];	// �X�e�[�W�ǂݍ��݂̃t�@�C����

	
	
	*/
};