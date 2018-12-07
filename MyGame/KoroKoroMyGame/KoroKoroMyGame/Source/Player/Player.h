//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_Player.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef PLAYER_H
#define PLAYER_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"

// ===== クラスの前方宣言 =====
class Collider;

// ===== クラス定義 =====
class Player final : public Pawn
{
public:
	Player();
	Player(D3DXVECTOR3 startPos,UINT setNumber);
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

	CHAR* ModelPenchanPass   = "Data/Model/Character/PenChan/PenguinC.x";
	CHAR* ModelPenNoHahaPass = "Data/Model/Character/PenNoHaha/PenguinA.x";

	void initialize();
	void finalize();
	void update(D3DXVECTOR3);
	void draw();

	virtual void initializeStatus();

	// シーン別更新
	void updateSceneEdit();
	void updateGameMain(D3DXVECTOR3);

	void setStatus(Player::PlayerState setStatus);
	void setScore(INT);

	void addScore();

	PlayerState getState()   const;
	INT  getScore() const;

private:
	static constexpr FLOAT VelocityForwardSpeed = 0.45f;
	static constexpr FLOAT VelocitySideSpeed	= 0.3f;
	static constexpr FLOAT ScaleSize		= 1.0f;
	static constexpr FLOAT MoveSpeed = 0.04f;

	void initializeTitle();
	void initializeSceneEdit();
	void initializeGameMain(CHAR* setFilePass);
	void initializeResult();
	void updateTitle(D3DXVECTOR3);
	void updateResult();
	void input();
	void move();

	void changeStatus();	// ステータス変更処理
	void changeState();		// 状態変更処理

	std::unique_ptr<Pawn> ballPtr;

	FLOAT rotCnt = 0.0f;

	D3DXVECTOR3	testVec;

	PlayerState	playerStateType;

	INT score;

	D3DXVECTOR3		oldPos;
	D3DXQUATERNION	startQuaternion;

};

#endif 
