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
#include "../Camera/Camera.h"
#include "../Collision/Collision.h"
#include <array>

// ===== 定数・マクロ定義 =====


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
	
	enum class MoveStateType
	{
		StartFade = 0,
		Normal,
		GoalFade,
	};

	void initialize();
	void initializeTitle();
	void initializeMain(Player *pPlayer);

	void finalize(Player* pPlayer);
	void finalize();
	void update(Player *,Board&);
	void updateTitle(Pawn*);
	void updateStageEdit(std::string keyName, UINT selectIndex);
	void updateGameMain(Player*,Board&);
	void setState(MoveStateType setState);

	void setCamera();

	D3DXVECTOR3  getRot();
	D3DXVECTOR3  getPos();
	D3DXVECTOR3  getLook();
	D3DXVECTOR3* getUp();

	D3DXMATRIX    getProjectionMtx();	// プロジェクションマトリックス取得
	D3DXMATRIX    getMtxView();			// ビューマトリックス取得
	D3DVIEWPORT9* getViwPort();			// ビューポートマトリックス取得

	D3DXVECTOR3 getFowerd();

#if _DEBUG
	void debugMove();
#endif

private:

	static constexpr FLOAT ViewAngle		= D3DXToRadian(55.0f);
	static constexpr FLOAT ViewAspect		= Application::ScreenWidth / Application::ScreenHeight;
	static constexpr FLOAT ViewNearZ		= 1.0f;
	static constexpr FLOAT ViewFarZ			= 20000.0f;
	
	static constexpr INT RotSpeed = 10;

	static constexpr size_t MaxFade = 3;

	MoveStateType myMoveType;

	CameraTransform myTransform;

	std::array<D3DXVECTOR3, MaxFade> fadePos;
	std::array<D3DXVECTOR3, MaxFade> fadeLook;

	INT currentFadeType = 0;

	void initializeStageEdit();					

	void rotationCamera(D3DXVECTOR3 Center);

	void Rotvelocity(D3DXVECTOR3* pVecCenter, FLOAT fRadius);

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
	D3DXMATRIX		mtxvelocity;				// カメラ移動行列

	FLOAT			radShake;				// 画面を揺らす際に用いるコサインカーブ用の角度
	INT				cntShakeFrame;			// カメラを揺らしているフレームのカウンタ
	D3DXVECTOR3		velocitycamera;				// カメラ座標の移動速度			

	FLOAT			rotRadian;				// 回転角度(ラジアン)
	FLOAT			rotPitchRadian;		// 回転角度(ラジアン)

	FLOAT			numvelocityPitch;			// ピッチの移動量
	D3DXVECTOR3		rotvelocityCamera;			// 角度からの移動方向ベクトル
	D3DXVECTOR3		velocityCameraDest;		// カメラの移動量

	D3DXVECTOR3		velocityPitchTest;	

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
	
	bool changeCamera;
	FLOAT lerpCnt;	// 線形補間用カウンタ

	INT   rotCnt;
	FLOAT rot;
};

#endif