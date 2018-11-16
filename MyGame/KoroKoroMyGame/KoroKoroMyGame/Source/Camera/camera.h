//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Camera.h
// Author : MasayaHayashi
// カメラクラス
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef CAMERA_H
#define CAMERA_H

// ===== インクルード部 ====
#include "d3dx9.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"
#include "../camera/camera.h"
#include <vector>

// ===== 定数・マクロ定義 =====
/*
#define	VIEW_ANGLE			(D3DXToRadian(55.0f))	// 視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z			(1.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(70000.0f)				// ビュー平面のFarZ値
#define	VALUE_MOVE_camera	(2.0f)					// カメラの移動量
#define	VALUE_ROTATE_camera	(D3DX_PI * 0.003809f)		// カメラの回転量

#define	INTERVAL_camera_L			(12.5f)			// モデルの視線の先までの距離
#define	INTERVAL_camera_L_MOVE		(20.5f)			// モデルの視線の先までの移動量

#define	CHASE_HEIGHT_P		(100.0f)				// 追跡時の視点の高さ
#define	CHASE_HEIGHT_L		(10.0f)					// 追跡時の注視点の高さ

#define RATE_CHASE_camera_P	(0.3f)					// カメラ視点への補正係数
#define RATE_CHASE_camera_L	(0.2f)					// カメラ注視点への補正係数

#define MAX_camera_FADE_POS (3)
*/

// ===== クラスの前方宣言 =====
class SceneManager;
class Pawn;
class Player;
class C_GAMEOBJ_BASE;
class Board;

// ===== クラス定義 =====
class Camera final
{
public:
	Camera();
	~Camera();

	enum class MoceType
	{
		Tutorial,
		Start,
		Normal,
		Goal,
	};

	void initialize();
	void initializeTitle(Player * pPlayer);
	void initializeMain(Player *pPlayer);

	void finalize(Player* pPlayer);
	void finalize();
	void update(Player *,Board *);
	void updateTitle(Pawn*);
	void updateStageEdit(const D3DXVECTOR3&);
	void updateGameMain(Player*,Board*);
	void setState(MoceType setState);


	void setCamera();

	D3DXVECTOR3  getRot();
	D3DXVECTOR3  getPos();
	D3DXVECTOR3  getLook();
	D3DXVECTOR3* getUp();

	D3DXMATRIX    getProjectionMtx();	// プロジェクションマトリックス取得
	D3DXMATRIX    getMtxView();			// ビューマトリックス取得
	D3DVIEWPORT9* getViwPort();			// ビューポートマトリックス取得

	D3DXVECTOR3 getFowerd();

private:
	/*
#define	VIEW_ANGLE			(D3DXToRadian(55.0f))	// 視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z			(1.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(70000.0f)				// ビュー平面のFarZ値
#define	VALUE_MOVE_camera	(2.0f)					// カメラの移動量
#define	VALUE_ROTATE_camera	(D3DX_PI * 0.003809f)		// カメラの回転量

#define	INTERVAL_camera_L			(12.5f)			// モデルの視線の先までの距離
#define	INTERVAL_camera_L_MOVE		(20.5f)			// モデルの視線の先までの移動量

#define	CHASE_HEIGHT_P		(100.0f)				// 追跡時の視点の高さ
#define	CHASE_HEIGHT_L		(10.0f)					// 追跡時の注視点の高さ

#define RATE_CHASE_camera_P	(0.3f)					// カメラ視点への補正係数
#define RATE_CHASE_camera_L	(0.2f)					// カメラ注視点への補正係数


// ===== 定数・マクロ定義 =====
#define MAX_PITCH_SPEED		(25.0f)	// ピッチの最大値
#define ACCELE_PITCH_SPEED	(0.5f)	// 1フレームに加速するピッチの値
#define DECELE_PITCH_SPEED	(0.25f)	// 1フレームに減速するピッチの値
*/

	static constexpr FLOAT ViewAngle	 = D3DXToRadian(55.0f);
	static constexpr FLOAT ViewAspect	 = Application::ScreenWidth / Application::ScreenHeight;
	static constexpr FLOAT ViewNearZ	 = 1.0f;
	static constexpr FLOAT ViewFarZ		 = 20000.0f;
	
	static constexpr INT RotSpeed = 10;

	enum class cameraState
	{
		Type3Person,
		Type1Person,
		TypePoint
	};

	void initializeStageEdit();					
//	void initializeGameMain(Player *pPlayer);

	void rotationCamera(D3DXVECTOR3 Center);


	D3DXVECTOR3 fadePos[3];
	D3DXVECTOR3 fadeLook[3];

	void RotMove(D3DXVECTOR3* pVecCenter, FLOAT fRadius);

	MoceType cameraMoveFade;

	SceneManager::SceneState currentScene;  // シーン識別用
	D3DXVECTOR3		cameraPos;				// カメラの視点
	D3DXVECTOR3		cameraLook;				// カメラの注視点
	D3DXVECTOR3		cameraUp;				// カメラの上方向

	D3DXVECTOR3		cameraPosDest;			// カメラの視点の目的位置
	D3DXVECTOR3		cameraLookDest;			// カメラの注視点の目的位置
	D3DXVECTOR3		cameraUpDest;			// カメラの上方向目的位置

	D3DXVECTOR3		cameraRot;				// カメラの回転角度
	FLOAT			lengthIntervalcamera;	// カメラの視点と注視点の距離
	FLOAT			heightcameraP;			// カメラの視点の高さ
	FLOAT			heightcameraL;			// カメラの注視点の高さ
	D3DXVECTOR3		cameraFowerd;			// カメラの前方向ベクトル
	D3DXMATRIX		mtxView;				// ビューマトリックス
	D3DXMATRIX		mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX		mtxRot;					// カメラ回転行列
	D3DXMATRIX		mtxMove;				// カメラ移動行列

	FLOAT			radShake;				// 画面を揺らす際に用いるコサインカーブ用の角度
	INT				cntShakeFrame;			// カメラを揺らしているフレームのカウンタ
	D3DXVECTOR3		movecamera;				// カメラ座標の移動速度			

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

	D3DXMATRIX rotcameraX;
	D3DXMATRIX rotcameraZ;

	D3DVIEWPORT9 viewPort;

	cameraState currentState;
	bool changeCamera;
	FLOAT lerpCnt;	// 線形補間用カウンタ

	INT   rotCnt;
	FLOAT rot;
};

#endif