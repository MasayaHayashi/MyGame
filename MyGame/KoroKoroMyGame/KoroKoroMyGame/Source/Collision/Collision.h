//������������������������������������������������������
// Collision.h
// 
// �����F
// �������邽�߂̍X�V�����Ȃǂ��܂Ƃ߂��N���X
// collider�ƍ������Ȃ��B
// 
// Author: Masaya Hayashi
//������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef COLLISION_H
#define COLLISION_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"

// ===== �񋓑̒�` =====

// ===== �\���̒�` =====
typedef struct
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
} SEGMENT;

typedef struct
{
	D3DXVECTOR3 P0;
	D3DXVECTOR3 P1;
	D3DXVECTOR3 P2;

} TRIANGLE;

// ===== �N���X�̑O����` =====
class C_3DPAWN;
class Collider;
class C_PLAYER;

// ===== �N���X��` =====
class C_COLLISION
{
public:
	C_COLLISION();	// �R���X�g���N�^
	~C_COLLISION();	// �f�X�g���N�^

	UINT CheckCollisionField(C_PLAYER *pPlayer, C_3DPAWN *pPawnB, C_3DPAWN *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);
	UINT CheckCollisionWall(C_PLAYER *pPlayer, C_3DPAWN *pPawnB, C_3DPAWN *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);

	void CheckCollisionBlock(C_3DPAWN *pSelectBlock, C_3DPAWN *pGameObj);

	bool IsHitSphereToSphere(C_3DPAWN *, C_3DPAWN*);					// ���Ƌ��̂����蔻��
	bool IsHitAABB(C_3DPAWN * ,C_3DPAWN *);								// AABB�̂����蔻��
	bool IsHitAABBItem(C_PLAYER *pPlayer, C_3DPAWN *pPawn);				// AABB�̃A�C�e���p����
	INT IsHitRayToMesh(C_3DPAWN *, C_3DPAWN *, LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 ,LPD3DXVECTOR3);			// ���C�ƎO�p�`�̂����蔻��

private:
	bool IntersectA(C_3DPAWN* pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld);
	INT  Intersect(C_3DPAWN *,LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 , LPD3DXVECTOR3 );
	void SwitchHitType(C_3DPAWN *, C_3DPAWN *);
};

#endif