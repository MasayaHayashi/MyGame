//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneMain.h
// シーンメインクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"
#include "../../DirectX3D/DirectX3D.h"
#include "../../Pawn/Pawn.h"
#include <memory>
#include <list>
#include <array>

/*
#include "Block.h"
#include "C_StarStar.h"
#include "C_StageEditor.h"
#include "C_ScoreUi.h"
*/

// ===== 列挙体定義 =====
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

// ===== クラスの前方宣言 =====
class Light;
class Camera;
class Player;
class Collision;
class BallObj;
class Board;
class Skydome;

// ===== クラス定義 =====
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
	void loadStageData(size_t stageNumber);

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

	const std::string StageFilePass = "Stage";

	static constexpr UINT	MaxGameObj = 1000;
	ExportData				exportWorkData;
	size_t					selectGameObjIndex;

	void checkUnProject(INT);
	void checkCollision();
	void setScore();
	void creteGameObj(size_t objType);

	static constexpr INT ReStartTime = 120;

	const std::string StageTutorialPass = "StageTutorial.bin";
	const std::string Stage1Pass		= "Stage1.bin";
	const std::string Stage2Pass		= "Stage2.bin";
	const std::string Stage3Pass		= "Stage3.bin";

	std::unique_ptr<Light>		lightPtr;
	std::unique_ptr<Camera>		cameraPtr;
	std::unique_ptr<Collision>	collisionPtr;

	std::list<BallObj*>														ballsPtr;
	std::list<std::unique_ptr<Player>>										playeresPtr;
	std::list<std::unique_ptr<Skydome>>										skyDomePtr;
	std::list<std::unique_ptr<Board>>										boardObjectesPtr;
	std::array<std::list<Pawn*>, GameObjectBase::MaxGameObjType>			gameObjPtr;

	GameState				currentGameState;
	static UINT				currentStage;
	static UINT				prevScore;


	/*
	Light					*pLight;											  // ライト
	C_CAMERA				*pCamera;											  // カメラ
	C_SKYDOME				*pSkydome;											  // スカイドーム
	Player				*pPlayer;											  // プレイヤー
	Collision				*pCollision;										  // 衝突用
	C_PAUSE					*pPause;											  // ポーズ用
	Pawn				*pGameObj[MAX_GAME_OBJ_TYPE][MAX_GAME_OBJ];			  // ゲームオブジェクト
	C_Star_BASE 		*pStarObj[MAX_Star_OBJ_TYPE][MAX_Star];   // パーティクル関連
	Board					*pUIObj[MAX_UI_OBJ_TYPE];							  // UI関連
	C_SCORE_UI				*pUIScore[MAX_SCORE_DIGIT];							  // スコア
	C_STAGE_LOADER			*pStageLoader;										  // ステージロード
	C_GOAL					*pGoal;												  // ゴール用
	C_HEART_BOARD			*pHeart;											  // ハート
	
	INT						nReStartCnt;								// 再スタートカウント

	UINT					uIsHitBlock;								// ブロックに当たっているか

	CHAR					szStageFileName[MAX_STAGE][MAX_FILE_NAME];	// ステージ読み込みのファイル名

	
	
	*/
};