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

// ===== 定数・マクロ定義 =====
#define PLAYER_SIDE_MOVE (5)

// ===== クラスの前方参照 =====
class Collider;

// ===== 構造体定義 =====

// ===== 列挙体定義 =====

	enum PLAYER_STATE
	{
		TYPE_STOP = 0,				// 停止
		TYPE_MOVE,					// 移動可能
		TYPE_MOVE_HIT_WALL,			// 移動できるが壁に当たっている
		TYPE_JUMP_UP,				// ジャンプ上昇中
		TYPE_JUMP_DOWN,				// ジャンプ下降中
		TYPE_FALL,					// 落下中(死亡へ)
		TYPE_DEAD,					// 死亡
		MAX_PLAYER_STATE,
	};


enum HIT_COLLISION_TYPE
{
	RAY_TRUE = 0,
	RAY_FALSE,
	NONE,
};

// ===== クラス定義 =====
class C_PLAYER final : public Pawn
{
public:
	C_PLAYER();
	~C_PLAYER();

	void InitObject();
	void UninitObject();
	void UpdateObject(D3DXVECTOR3);
	void DrawObject();

	virtual void InitStatus();

	void UpdatePlayer_SceneEdit();
	void UpdatePlayer_GameMain(D3DXVECTOR3);

	void SetStatus(PLAYER_STATE SetStatus);
	PLAYER_STATE GetState();
	D3DXVECTOR3 GetMoveVec();

	void SetScore(INT);
	void AddScore();
	INT  GetScore();
private:

	void InitPlayer_Title();
	void InitPlayer_SceneEdit();
	void InitPlayer_GameMain();
	void InitPlayer_Result();
	void UpdatePlayer_Title(D3DXVECTOR3);
	void UpdatePlayer_Result();


	void ChangeStatus();	// ステータス変更処理
	void ChangeState();		// 状態変更処理

	
	PLAYER_STATE				 PlayerState;	// プレイヤーの状態



	INT nScore;					// スコア

	D3DXVECTOR3		OldPos;
	D3DXVECTOR3		TestVec;
	D3DXQUATERNION	StartQua;

};

#endif 
