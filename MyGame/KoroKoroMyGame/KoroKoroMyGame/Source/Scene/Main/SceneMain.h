//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneMain.h
// シーンメインクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"
#include "../../DirectX3D/DirectX3D.h"
#include <memory>

/*
#include "C_Block.h"
#include "C_StarParticle.h"
#include "C_StageEditor.h"
#include "C_ScoreUi.h"
*/

// ===== 列挙体定義 =====
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

// ===== 定数・マクロ定義 =====

// ===== クラスの前方宣言 =====
class Light;
class Camera;
class Player;

// ===== クラス定義 =====
class SceneMain : public C_SCENE_BASE
{
public:
	SceneMain();
	~SceneMain();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

	// ゲッター関連
	Camera* getCamera();

private:
	// ===== 列挙体定義 =====
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
	GameState getGameState();

	static constexpr INT RestartTime = 120;

	const std::string StageTutorialPass = "StageTutorial.bin";
	const std::string Stage1Pass		= "Stage1.bin";
	const std::string Stage2Pass		= "Stage2.bin";
	const std::string Stage3Pass		= "Stage3.bin";

	std::unique_ptr<Light>  lightPtr;
	std::unique_ptr<Camera> cameraPtr;
	std::unique_ptr<Player> playerPtr;


	GameState				currentGameState;
	static UINT				currentStage;
	static UINT				prevScore;

	/*

	Light					*pLight;											  // ライト
	C_CAMERA				*pCamera;											  // カメラ
	C_SKYDOME				*pSkydome;											  // スカイドーム
	Player				*pPlayer;											  // プレイヤー
	C_COLLISION				*pCollision;										  // 衝突用
	C_PAUSE					*pPause;											  // ポーズ用
	Pawn				*pGameObj[MAX_GAME_OBJ_TYPE][MAX_GAME_OBJ];			  // ゲームオブジェクト
	C_PARTICLE_BASE 		*pParticleObj[MAX_PARTICLE_OBJ_TYPE][MAX_PARTICLE];   // パーティクル関連
	Board					*pUIObj[MAX_UI_OBJ_TYPE];							  // UI関連
	C_SCORE_UI				*pUIScore[MAX_SCORE_DIGIT];							  // スコア
	C_STAGE_LOADER			*pStageLoader;										  // ステージロード
	C_GOAL					*pGoal;												  // ゴール用
	C_HEART_BOARD			*pHeart;											  // ハート
	
	INT						nRestartCnt;								// 再スタートカウント

	UINT					uIsHitBlock;								// ブロックに当たっているか

	CHAR					szStageFileName[MAX_STAGE][MAX_FILE_NAME];	// ステージ読み込みのファイル名

	
	
	*/
};