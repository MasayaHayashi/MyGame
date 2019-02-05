//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_Player.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef PLAYER_H
#define PLAYER_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"
#include "../SelectManager/SelectManager.h"

// ===== クラスの前方宣言 =====
class Collider;
class BallObj;
class SelectManager;

// ===== クラス定義 =====
class Player final : public Pawn
{
public:
	Player();
	Player(D3DXVECTOR3 StartPos,UINT setNumber);
	~Player();
	
	enum class PlayerState
	{
		Stop = 0,
		velocity,
		velocityHitWall,
		JumpUp,
		JumpDown,
		Fall,
		Dead
	};

	void initialize();
	void finalize();
	void update(D3DXVECTOR3);
	void draw();

	virtual void initializeStatus();

	void updateStageEdit();
	void updateGameMain(D3DXVECTOR3);
	void updateGoal();
	void updateSelect();
	void updateMiss();

	void setStatus(Player::PlayerState setStatus);
	void setScore(INT);

	void addScore();

	void changeModel();
	void updatePlayng(D3DXVECTOR3 CameraForward);
	void updateReady(D3DXVECTOR3 CameraForward);


	PlayerState getState()   const;
	INT  getScore() const;

private:
	
	static constexpr FLOAT  FallSpeed				= 0.01f;
	static constexpr FLOAT  VelocityForwardSpeed	= 0.45f;
	static constexpr FLOAT  VelocitySideSpeed		= 0.3f;
	static constexpr FLOAT  ScaleSize				= 1.0f;
	static constexpr FLOAT  MoveSpeed				= 0.2f;
	static constexpr FLOAT  SideSpeed				= 0.3f;
	static constexpr FLOAT  MaxSpeed				= 0.02f;
	static constexpr FLOAT  JumpSpeed				= 0.14f;
	static constexpr FLOAT  MissSpeed			    = 0.2f;
	static constexpr FLOAT  FallMissPosY			= 11.0f;


	bool isKeyInput = false;
	bool fallFlg	= true;

	D3DXVECTOR3 inputVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	void initializeTitle();
	void initializeSceneEdit();
	void initializeGameMain();
	void initializeResult();
	void updateTitle(D3DXVECTOR3);
	void updateResult();
	void input();
	void move();
	void rideBall(size_t setIndex);
	void fall(size_t checkIndex);
	void rotation(D3DXVECTOR3 destVec);
	bool isFall();

	void rebound(size_t index);

	void changeStatus();
	void changeState();

	FLOAT rotCnt = 0.0f;

	D3DXVECTOR3	moveVector;

	PlayerState	playerStateType;
	bool missFlg = false;
	INT score;
	FLOAT radianRotCnt = 0;

	FLOAT Xnum = 0.0f;
	FLOAT Ynum = 0.0f;

	D3DXVECTOR3		oldPos;
	D3DXQUATERNION	StartQuaternion;

	D3DXVECTOR3 oldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	UINT currentModelType = 0;

};

#endif 
