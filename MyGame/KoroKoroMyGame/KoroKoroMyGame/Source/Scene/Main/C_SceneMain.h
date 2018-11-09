//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneMain.h
// シーンメインクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "C_SceneBase.h"
#include "C_Block.h"
#include "C_StarParticle.h"
#include "C_StageEditor.h"
#include "C_ScoreUi.h"

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

// ===== クラスの前方参照 =====
class Light;
class C_CAMERA;
class C_PLAYER;
class C_SKYDOME;
class C_ENEMY;
class C_PAUSE_UI_OBJ;
class C_PAUSE;
class C_BLOCK;
class C_BOARD;
class C_STAGE_LOADER;
class C_GOAL;
class C_HEART_BOARD;

// ===== クラス定義 =====
class C_SCENE_MAIN : public C_SCENE_BASE
{
public:
	C_SCENE_MAIN();
	~C_SCENE_MAIN();

	void InitScene();
	void finalizeScene();
	void updateScene();
	void drawScene();

	void InitStatus();

	// ゲッター関連
	C_CAMERA* getCamera();

private:

	void CheckUnProject(INT);
	void CheckCollision();
	void setScore();

	Light					*pLight;											  // ライト
	C_CAMERA				*pCamera;											  // カメラ
	C_SKYDOME				*pSkydome;											  // スカイドーム
	C_PLAYER				*pPlayer;											  // プレイヤー
	C_COLLISION				*pCollision;										  // 衝突用
	C_PAUSE					*pPause;											  // ポーズ用
	Pawn				*pGameObj[MAX_GAME_OBJ_TYPE][MAX_GAME_OBJ];			  // ゲームオブジェクト
	C_PARTICLE_BASE 		*pParticleObj[MAX_PARTICLE_OBJ_TYPE][MAX_PARTICLE];   // パーティクル関連
	C_BOARD					*pUIObj[MAX_UI_OBJ_TYPE];							  // UI関連
	C_SCORE_UI				*pUIScore[MAX_SCORE_DIGIT];							  // スコア
	C_STAGE_LOADER			*pStageLoader;										  // ステージロード
	C_GOAL					*pGoal;												  // ゴール用
	C_HEART_BOARD			*pHeart;											  // ハート

	INT						nRestartCnt;								// 再スタートカウント
	static UINT				uCurrentStage;								// 現在のステージ

	UINT					uIsHitBlock;								// ブロックに当たっているか

	CHAR					szStageFileName[MAX_STAGE][MAX_FILE_NAME];	// ステージ読み込みのファイル名

	UINT					uPrevScore;									// 1フレーム前のスコア


};