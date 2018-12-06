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
#include <list>

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

// ===== �N���X�̑O���錾 =====
class Pawn;
class Collider;
class Player;

// ===== �N���X��` =====
class Collision
{
public:
	Collision();	
	~Collision();	

	void update();

	static void registerList(Transform *setPawn,std::string keyName);

	bool checkCollisionField(Pawn *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);
	UINT CheckCollisionWall( Player *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec);

	void CheckCollisionBlock(Pawn *pSelectBlock, Pawn *pGameObj);

	bool IsHitSphereToSphere(Pawn *, Pawn*);					// ���Ƌ��̂����蔻��
	bool isHitAABB(const Pawn pawnA, const Pawn pawnB) const;
	bool IsHitAABBItem(Player *pPlayer, Pawn *pPawn);				// AABB�̃A�C�e���p����
	INT isHitRayToMesh(Pawn*, Pawn*, LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 ,LPD3DXVECTOR3);			// ���C�ƎO�p�`�̂����蔻��
	
	static const Transform* getTransform(std::string keyName, INT index);

private:
	bool IntersectA(Pawn* pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld);
	INT  Intersect(Pawn*,LPD3DXVECTOR3 , LPD3DXVECTOR3 , bool , LPD3DXVECTOR3 , LPD3DXVECTOR3 , LPD3DXVECTOR3 );
	void SwitchHitType(Pawn *, Pawn *);

	void checkPlayerCollision();

	static std::unordered_map<std::string, Transform*> collisionMapes;
};

#endif