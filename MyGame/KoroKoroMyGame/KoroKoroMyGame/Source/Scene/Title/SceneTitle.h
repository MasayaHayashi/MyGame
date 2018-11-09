//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneTitle.h
// Author	 : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENE_TITLE
#define SCENE_TITLE

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"

#include "C_Skydome.h"
#include "C_ParticleBase.h"

// ===== 列挙体定義 =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== クラスの前方定義 =====
class C_MAIN_FIELD;
class C_PLAYER;
class C_TITLE_UI;
class C_PARTICLE_BASE;
class C_TITLE_OBJ;
class C_CAMERA;
class C_LIGHT;
class C_FADE;

// ===== クラス定義 =====
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
	C_CAMERA				*pCamera;							// カメラ
	C_LIGHT					*pLight;							// ライト
	C_SKYDOME				*pSkydome;							// スカイドーム



	C_PLAYER				*pPlayer;							// プレイヤー
	C_MAIN_FIELD			*pField;							// フィールド
	C_TITLE_UI				*pTitleUI;							// タイトルUI
	C_TITLE_OBJ				*pTitleObj;							// タイトルオブジェクト
	C_BOARD					*pBoard[MAX_UI_TYPE];				// UI関連
	C_PARTICLE_BASE			*pParticle[MAX_PARTICLE];			// パーティクル

	bool					bChangeScene;						// シーン切り替え
	INT						nChangeSceneWaitCnt;				// 待機用カウンタ
	UINT					uSelectScene;

};

#endif