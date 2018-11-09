//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneTitle.h
// Author	 : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENE_TITLE
#define SCENE_TITLE

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"
#include "../../Skydome/Skydome.h"

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
class Light;
class C_FADE;
class Board;

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

	const camera* getcamera();


private:
	static constexpr UINT MaxUIType = static_cast<UINT>(UIType::DescUI) + 1;

	camera					*pcamera;							// カメラ
	Light					*pLight;							// ライト

	Skydome					*pSkydome;							// スカイドーム


	C_PLAYER				*pPlayer;
	C_MAIN_FIELD			*pField;
	C_TITLE_UI				*pTitleUI;							// タイトルUI
	C_TITLE_OBJ				*pTitleObj;							// タイトルオブジェクト
	Board					*pBoard[MaxUIType];					// UI関連
//	C_PARTICLE_BASE			*pParticle[MAX_PARTICLE];			// パーティクル

	bool					bChangeScene;						// シーン切り替え
	INT						nChangeSceneWaitCnt;				// 待機用カウンタ
	UINT					uSelectScene;

};

#endif