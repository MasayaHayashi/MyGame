//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneLoad.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef C_SCENE_LOAD_H
#define C_SCENE_LOAD_H

// ===== インクルード部 =====
#include "../SceneInterface/sceneBase.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方定義 =====
class C_LOAD_UI_ICON;
class Light;
class C_camera;
class C_TITLE_OBJ;
class C_FADE;

// ===== 列挙体定義 =====
enum LOAD_STATE
{
	LOADING = 0,
	LOAD_COMP,
	MAX_LOAD_STAGE,
};

// ===== クラス定義 =====
class C_SCENE_LOAD
{
public:
	C_SCENE_LOAD();
	~C_SCENE_LOAD();

	void InitScene();
	void finalizeScene();
	void EnableLoad();
	void updateLoad();
	void drawScene();

	// セッター関連

	// 静的メンバ関数
	static void setLoadFlg(bool);

private:
	std::mutex mutex;

	C_camera		*pcamera;
	Light			*pLight;
	C_TITLE_OBJ		*pTitleObj;
	C_FADE			*pFade;
	C_LOAD_UI_ICON	*pLoadIcon;

	// 静的メンバ変数
	static bool	   finishedLoad;
};

#endif