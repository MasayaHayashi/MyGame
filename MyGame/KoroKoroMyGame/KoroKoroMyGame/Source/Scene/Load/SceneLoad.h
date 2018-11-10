//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneLoad.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENE_LOAD_H
#define SCENE_LOAD_H

// ===== インクルード部 =====
#include "../SceneInterface/sceneBase.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方定義 =====
class C_LOAD_UI_ICON;
class C_LIGHT;
class C_CAMERA;
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
class SceneLoad
{
public:
	SceneLoad();
	~SceneLoad();

	static void initialize();
	static void finalize();
	static void enable();
	static void update();
	static void draw();

	static void setLoadFlg(bool);

private:
	C_CAMERA		*pCamera;
	C_LIGHT			*pLight;
	C_TITLE_OBJ		*pTitleObj;
	C_FADE			*pFade;
	C_LOAD_UI_ICON	*pLoadIcon;

	// 静的メンバ変数
	static bool	   finishedLoad;					// ロード終了
};

#endif