//������������������������������������������������������������������������������������������������������
// C_Player.cpp
// Author : Masaya Hayashi
//������������������������������������������������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef PLAYER_H
#define PLAYER_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"
#include "../SelectManager/SelectManager.h"

// ===== �N���X�̑O���錾 =====
class Collider;
class BallObj;
class SelectManager;

// ===== �N���X��` =====
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
	void updateSelect();

	void setStatus(Player::PlayerState setStatus);
	void setScore(INT);

	void addScore();

	void changeModel();

	PlayerState getState()   const;
	INT  getScore() const;

private:
	
	static constexpr FLOAT FallSpeed			= 0.01f;
	static constexpr FLOAT VelocityForwardSpeed = 0.45f;
	static constexpr FLOAT VelocitySideSpeed	= 0.3f;
	static constexpr FLOAT ScaleSize			= 1.0f;
	static constexpr FLOAT MoveSpeed			= 0.0004f;
	static constexpr FLOAT MaxSpeed				= 0.02f;

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

	void rebound(size_t index);

	void changeStatus();	// �X�e�[�^�X�ύX����
	void changeState();		// ��ԕύX����

	FLOAT rotCnt = 0.0f;

	D3DXVECTOR3	moveVector;

	PlayerState	playerStateType;

	INT score;
	FLOAT Xnum = 0.0f;
	FLOAT Ynum = 0.0f;

	D3DXVECTOR3		oldPos;
	D3DXQUATERNION	StartQuaternion;

	UINT currentModelType = 0;

};

#endif 
