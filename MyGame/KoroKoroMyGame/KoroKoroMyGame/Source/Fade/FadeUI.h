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

// ===== 前方宣言 =====
class SceneManager;
enum class SceneState;

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

	FadeUI();
	~FadeUI();

	void initialize();
	void update();
	const void draw();
	void finalize();

	void setColor(D3DCOLOR col);
	void setFade(FadeType Fade);
	void setWorkScene(SceneManager::SceneState);
	
	FadeType getFadeState();


protected:
private:
	std::string TextureName = "data/TEXTURE/test.png";


	SceneManager::SceneState CurrentScene;
	FadeType  CurentFadeType;		// 現在のフェード状態
	D3DXCOLOR CurrentColor;			// 現在色状態
	SceneManager::SceneState WorkScene;
};

#endif
