//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// FadeUI.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef FADE_UI
#define FADE_UI

// ===== インクルード部 =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"

// ===== 定数・マクロ定義 =====

// ===== クラス定義 =====
class FadeUI final : public Board
{
public:
	enum class FadeType
	{
		FadeNone = 0,		// 何もない状態
		FadeIn,			// フェードイン処理
		FadeOut,			// フェードアウト処理
		FadeComp,			// フェード完了
	};

	FadeUI();			// コンストラクタ
	~FadeUI();			// デストラクタ

	void initialize();
	void Update();
	void Draw();
	void Uninit();

	void SetColor(D3DCOLOR col);
	void SetFade(FadeType Fade);
	void SetWorkScene(C_SCENE_MANAGER::SCENE_STATE);
	
	FadeType GetFadeState();


protected:
private:

	SceneManager::SCENE_STATE CurrentScene;
	FadeType  CurentFadeType;		// 現在のフェード状態
	D3DXCOLOR CurrentColor;			// 現在色状態
	SceneManager::SCENE_STATE WorkScene;
};

#endif
