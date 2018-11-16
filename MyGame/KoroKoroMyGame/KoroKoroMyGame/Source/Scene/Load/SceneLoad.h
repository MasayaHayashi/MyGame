//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneLoad.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENE_LOAD_H
#define SCENE_LOAD_H

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方定義 =====
class MyLoadIcon;
class C_TITLE_OBJ;
class C_FADE;

class Camera;
class Light;

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
	static void update();
	static void draw();

	static void enable();

	static void setLoadFlg(bool);

private:
	static std::unique_ptr<Camera>			cameraPtr;
	static std::unique_ptr<Light>			lightPtr;
	static std::unique_ptr<MyLoadIcon>		myLoadIConPtr;

	static bool	   isFinishedLoad;					// ロード終了
};

#endif