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

// ===== 構造体定義 =====


enum HIT_COLLISION_TYPE
{
	RAY_TRUE = 0,
	RAY_FALSE,
	NONE,
};

// ===== クラス定義 =====
class Player final : public Pawn
{
public:
	Player();
	~Player();

	enum class PlayerState
	{
		Stop = 0,	
		Move,
		MoveHitWall,			// 移動できるが壁に当たっている
		JumpUp,				// ジャンプ上昇中
		JumpDown,			// ジャンプ下降中
		Fall,
		Dead
	};

	void initialize();
	void finalize();
	void update(D3DXVECTOR3);
	void draw();

	virtual void initializeStatus();

	void UpdatePlayer_SceneEdit();
	void UpdatePlayer_GameMain(D3DXVECTOR3);

	void setStatus(Player::PlayerState setStatus);
	PlayerState getState();
	D3DXVECTOR3 getMoveVec();

	void SetScore(INT);
	void AddScore();
	INT  GetScore();
private:
	//.. / Data / Model / Character / PenChan /
	const CHAR* ModelFilePass = "Data/Model/Character/PenChan/PenguinC.x";

	void initializeTitle();
	void initializeSceneEdit();
	void initializeGameMain();
	void initializeResult();
	void updateTitle(D3DXVECTOR3);
	void updateResult();


	void ChangeStatus();	// ステータス変更処理
	void ChangeState();		// 状態変更処理
	
	PlayerState				 playerStateType;

	INT nScore;

	D3DXVECTOR3		OldPos;
	D3DXVECTOR3		TestVec;
	D3DXQUATERNION	StartQua;

};

#endif 
