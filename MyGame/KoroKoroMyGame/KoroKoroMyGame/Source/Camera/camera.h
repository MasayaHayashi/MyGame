//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_Camera.h
// Author : MasayaHayashi
// カメラクラス
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef C_CAMERA_H
#define C_CAMERA_H

// ===== インクルード部 ====
#include "d3dx9.h"
#include "../SceneManager/SceneManager.h"
#include "../Camera/camera.h"
#include <vector>

// ===== 定数・マクロ定義 =====
#define	VIEW_ANGLE			(D3DXToRadian(55.0f))	// 視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z			(1.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(70000.0f)				// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA	(2.0f)					// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.003809f)		// カメラの回転量

#define	INTERVAL_CAMERA_L			(12.5f)			// モデルの視線の先までの距離
#define	INTERVAL_CAMERA_L_MOVE		(20.5f)			// モデルの視線の先までの移動量

#define	CHASE_HEIGHT_P		(100.0f)				// 追跡時の視点の高さ
#define	CHASE_HEIGHT_L		(10.0f)					// 追跡時の注視点の高さ

#define RATE_CHASE_CAMERA_P	(0.3f)					// カメラ視点への補正係数
#define RATE_CHASE_CAMERA_L	(0.2f)					// カメラ注視点への補正係数

#define MAX_CAMERA_FADE_POS (3)

// ===== 列挙体定義 =====
enum CAMERA_MOVE_TYPE
{
	MAIN_TUTORIAL,
	MAIN_START_FADE,
	MAIN_NORMAL,
	MAIN_GOAL_FADE,
	MAX_CAMERA_MOVE_TYPE,
};

// ===== クラスの前方宣言 =====
class SceneManager;
class Pawn;

class C_PLAYER;
class C_GAMEOBJ_BASE;
class C_BOARD;

// ===== クラス定義 =====
class Camera
{
public:
	Camera();
	~Camera();

	void initialize();
	void finalize(C_PLAYER* pPlayer);
	void finalize();
	void update(C_PLAYER *,C_BOARD *);
	void updateTitle(Pawn*);
	void updateStageEdit(const D3DXVECTOR3&);
	void updateGameMain(C_PLAYER*,C_BOARD*);
	void setState(CAMERA_MOVE_TYPE setState);


	void setCamera();

	D3DXVECTOR3  getRot();
	D3DXVECTOR3  getPos();
	D3DXVECTOR3  getLook();
	D3DXVECTOR3* getUp();

	D3DXMATRIX    getProjectionMtx();	// プロジェクションマトリックス取得
	D3DXMATRIX    getMtxView();			// ビューマトリックス取得
	D3DVIEWPORT9* getViwPort();			// ビューポートマトリックス取得

	D3DXVECTOR3 getCameraFowerd();

private:
	enum class CameraState
	{
		Type3Person,
		Type1Person,
		TypePoint
	};

	static constexpr FLOAT CameraRotSpeed = 22.0f;

	void initTitle(C_PLAYER * pPlayer);		// タイトル画面カメラ初期化
	void initStageEdit();					// ステージエディット用カメラ初期化
	void initGameMain(C_PLAYER *pPlayer);	// ゲームメイン用初期化

	void rotationCamera(D3DXVECTOR3 Center);


	D3DXVECTOR3 fadePos[MAX_CAMERA_FADE_POS];
	D3DXVECTOR3 fadeLook[MAX_CAMERA_FADE_POS];

	void RotMove(D3DXVECTOR3* pVecCenter, FLOAT fRadius);

	CAMERA_MOVE_TYPE cameraMoveFade;

	SceneManager::SceneState currentScene;  // シーン識別用
	D3DXVECTOR3		cameraPos;				// カメラの視点
	D3DXVECTOR3		cameraLook;				// カメラの注視点
	D3DXVECTOR3		cameraUp;				// カメラの上方向

	D3DXVECTOR3		cameraPosDest;			// カメラの視点の目的位置
	D3DXVECTOR3		cameraLookDest;			// カメラの注視点の目的位置
	D3DXVECTOR3		cameraUpDest;			// カメラの上方向目的位置

	D3DXVECTOR3		cameraRot;				// カメラの回転角度
	FLOAT			lengthIntervalCamera;	// カメラの視点と注視点の距離
	FLOAT			heightCameraP;			// カメラの視点の高さ
	FLOAT			heightCameraL;			// カメラの注視点の高さ
	D3DXVECTOR3		cameraFowerd;			// カメラの前方向ベクトル
	D3DXMATRIX		mtxView;				// ビューマトリックス
	D3DXMATRIX		mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX		mtxRot;					// カメラ回転行列
	D3DXMATRIX		mtxMove;				// カメラ移動行列

	FLOAT			radShake;				// 画面を揺らす際に用いるコサインカーブ用の角度
	INT				cntShakeFrame;			// カメラを揺らしているフレームのカウンタ
	D3DXVECTOR3		moveCamera;				// カメラ座標の移動速度			

	FLOAT			rotRadian;				// 回転角度(ラジアン)
	FLOAT			rotPitchRadian;		// 回転角度(ラジアン)

	FLOAT			numMovePitch;			// ピッチの移動量
	D3DXVECTOR3		rotMoveCamera;			// 角度からの移動方向ベクトル
	D3DXVECTOR3		moveCameraDest;		// カメラの移動量

	D3DXVECTOR3		movePitchTest;	

	D3DXVECTOR3		rotWorkX;
	D3DXVECTOR3		rotWorkY;
	D3DXVECTOR3		rotWorkZ;

	D3DXMATRIX mW, mY, mX, mZ;
	D3DXVECTOR3 vX;
	D3DXVECTOR3 vY;
	D3DXVECTOR3 vZ;
	D3DXVECTOR3 vP;

	D3DXMATRIX rotCameraX;
	D3DXMATRIX rotCameraZ;

	D3DVIEWPORT9 viewPort;

	CameraState currentState;
	bool changeCamera;
	FLOAT lerpCnt;	// 線形補間用カウンタ

	INT   rotCnt;
	FLOAT rot;
};

#endif