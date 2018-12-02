//��������������������������������������
// Collision.cpp
//
// �����F
// �������邽�߂̍X�V�����Ȃǂ��܂Ƃ߂��N���X
// collider�N���X�Ƃ͕�
//
// Auhotor : Masaya Hayashi
//��������������������������������������

// ===== �C���N���[�h�� =====
#include "Collision.h"
#include "../Player/Player.h"
#include "../Pawn/Pawn.h"
#include "../DirectX3D/DirectX3D.h"

// ===== �ÓI�����o =====
std::unordered_map<std::string, Transform> Collision::collisionMapes;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
Collision::Collision()
{
	
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
Collision::~Collision()
{

}

//������������������������������������
// �X�V
//������������������������������������
void Collision::update()
{
 	FLOAT test = collisionMapes["Player"].getData(0)->posData.x;
	DirectX3D::printDebug("posdata %f\n", test);

	if (isHitAABB(*collisionMapes["Player"].getData(0), *collisionMapes["Player"].getData(1)))
	{
		collisionMapes["Player"].setHit(0, true);
		collisionMapes["Player"].setHit(1, true);
		DirectX3D::printDebug("������");
	}
	else
	{
		collisionMapes["Player"].setHit(0, false);
		collisionMapes["Player"].setHit(1, false);
		DirectX3D::printDebug("���܂���");
	}
}

//������������������������������������
// �o�^
//������������������������������������
void Collision::registerList(TransformData *setTransformData,std::string keyName)
{
	collisionMapes[keyName].setData(setTransformData);
}

//������������������������������������
// �n�ʂƃv���C���[�̏Փˏ����X�V
//������������������������������������
UINT Collision::CheckCollisionField(Player *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength,D3DXVECTOR3 DestVec)
{
	INT	nIndx;

	// �����ƎO�p�`�̔���
	nIndx = IsHitRayToMesh(pField, pPlayer, &pPlayer->getPosition(), &(pPlayer->getPosition() + DestVec), true, &Cross, &Normal,&fLength);

	if (nIndx >= 0)
	{
		return RAY_TRUE;
	}
	else
	{
		return RAY_FALSE;
	}
}

//������������������������������������
// �ǂƃv���C���[�̏Փˏ����X�V
//������������������������������������
UINT Collision::CheckCollisionWall(Player *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec)
{
	INT	nIndx;

	// �����ƎO�p�`�̔���
	nIndx = IsHitRayToMesh(pField, pPlayer, &(pPlayer->getPosition() + D3DXVECTOR3(0.0f,2.0f,0.0f)), &(pPlayer->getPosition() + DestVec), true, &Cross, &Normal, &fLength);

	if (nIndx >= 0)
	{
		return RAY_TRUE;
	}
	else
	{
		return RAY_FALSE;
	}
}


//��������������������������������������������
// �Փˏ���(�폜�p) (�I�u�W�F�N�g�ƃI�u�W�F�N�g)
//��������������������������������������������
void Collision::CheckCollisionBlock(Pawn *pSelectBlock, Pawn *pGameObj)
{
	/*
	if (IsHitAABB(pSelectBlock, pGameObj))
	{
		pGameObj->setPosition( D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pGameObj->setUsedFlg(false);
	}
	*/
}

//��������������������������������������������������������������������������
// ���Ƌ��̂����蔻��
//��������������������������������������������������������������������������
bool Collision::IsHitSphereToSphere(Pawn *pPawnA,Pawn *pPawnB)
{
	D3DXVECTOR3 SphereA = pPawnA->getOffset();
	D3DXVECTOR3 SphereB = pPawnB->getPosition();
	FLOAT fRadA = pPawnA->getCollisionRadius() * 1.5f;
	FLOAT fRadB = pPawnB->getCollisionRadius() * 1.5f;

	if (   (SphereA.x - SphereB.x) * (SphereA.x - SphereB.x)
		+  (SphereA.y - SphereB.y) * (SphereA.y - SphereB.y)
		+  (SphereA.z - SphereB.z) * (SphereA.z - SphereB.z)
		<= (fRadA + fRadB) * (fRadA + fRadB))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
//������������������������������������������������������������
// ���@�� : �����i�L���j�Ɩ@���t���O�p�`�̓�����`�F�b�N
// ���@�� : _segment		�����̐����istart��end���������j
//			_tri		�@���t���O�p�`
//			_inter		��_�i�ȗ��j
// �߂�l : �������Ă��邩�ۂ�
// ���@�� : ���ʂ̓�����͂Ƃ�Ȃ�
//--------------------------------------------------------------------------------------------
bool CheckSegment2Triangle(const SEGMENT& _segment, const TRIANGLE& _triangle, D3DXVECTOR3 *_inter)
{
	const float epsilon = -1.0e-5f;	// �덷�z���p�̔����Ȓl
	D3DXVECTOR3 	LayV;		// �����̏I�_���n�_
	D3DXVECTOR3 	tls;		// �O�p�`�̒��_0�������̎n�_
	D3DXVECTOR3 	tle;		// �O�p�`�̒��_0�������̏I�_
	float 	distl0;
	float 	distl1;
	float 	dp;
	float 	denom;
	float 	t;
	D3DXVECTOR3	s;			// �����ƕ��ʂƂ̌�_
	D3DXVECTOR3 	st0;		// ��_���O�p�`�̒��_0
	D3DXVECTOR3 	st1;		// ��_���O�p�`�̒��_1
	D3DXVECTOR3 	st2;		// ��_���O�p�`�̒��_2
	D3DXVECTOR3 	t01;		// �O�p�`�̒��_0�����_1
	D3DXVECTOR3 	t12;		// �O�p�`�̒��_1�����_2
	D3DXVECTOR3 	t20;		// �O�p�`�̒��_2�����_0
	D3DXVECTOR3	m;

	// �����̎n�_���O�p�n�̗����ɂ���΁A������Ȃ�
	tls = _segment.start - _triangle.P0;

	D3DXVECTOR3 test;
	D3DXVec3Normalize(&test,&tls);

	distl0 = D3DXVec3Dot(&tls, &test);

//	distl0 = tls.Dot(_triangle.Normal);	// �����̎n�_�ƕ��ʂ̋���
	if (distl0 <= epsilon) return false;

	// �����̏I�_���O�p�n�̕\���ɂ���΁A������Ȃ�
	tle = _segment.end - _triangle.P0;

	D3DXVec3Normalize(&test, &tle);

	distl1 = D3DXVec3Dot(&test, _triangle);

//	distl1 = tle.Dot(_triangle.Normal);	// �����̏I�_�ƕ��ʂ̋���
	if (distl1 >= -epsilon) return false;

	// �����ƕ��ʂƂ̌�_s�����
	denom = distl0 - distl1;
	t = distl0 / denom;
	LayV = _segment.end - _segment.start;	// �����̕����x�N�g�����擾
	s = t * LayV + _segment.start;

	// ��_���O�p�`�̓����ɂ��邩�ǂ����𒲂ׂ�B
	// �O�p�`�̓����ɂ���ꍇ�A��_����e���_�ւ̃x�N�g���Ɗe�Ӄx�N�g���̊O�ρi�O�g�j���A�S�Ė@���Ɠ�������������
	// ��ł���������v���Ȃ���΁A������Ȃ��B
	st0 = _triangle.P0 - s;
	t01 = _triangle.P1 - _triangle.P0;
	
	D3DXVec3Cross(&m, &st0, &t01);
//	m = st0.Cross(t01);

	dp = D3DXVec3Dot(&m, &_triangle);

//	dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	st1 = _triangle.P1 - s;
	t12 = _triangle.P2 - _triangle.P1;

	D3DXVec3Cross(&m, &st1, &t12);
	//m = st1.Cross(t12);

	dp = D3DXVec3Dot(&m,&_triangle);
	//dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	st2 = _triangle.P2 - s;
	t20 = _triangle.P0 - _triangle.P2;

	D3DXVec3Cross(&m, &st2, &t20);
	//m = st2.Cross(t20);
	
	dp = D3DXVec3Dot(&m, &_triangle);
//	dp = m.Dot(_triangle.Normal);
	if (dp <= epsilon) return false;

	if (_inter)
	{
		*_inter = s;	// ��_���R�s�[
	}

	return true;
}
*/

//��������������������������������������������������������������������
// ���C�ƎO�p�`�̂����蔻��
// ���� : 
//		  pPawnA  : �t�B�[���h�̃��b�V�����
//		  pPawnB  : �v���C���[�̃��b�V�����
//		  pCross  : �����������W 
//		  pNormal : �@���x�N�g��
//		  fLength : �ʒu����̒���
// �߂�l:
//		  �������Ă���		�� �������Ă���O�p�`�̓Y����
//		  �������Ă��Ȃ�	�� -1
//��������������������������������������������������������������������
INT Collision::IsHitRayToMesh(Pawn *pPawnA, Pawn *pPawnB, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal,LPD3DXVECTOR3 Length)
{
		// ���[���h�}�g���b�N�X�̋t�}�g���b�N�X�𐶐�
		D3DXMATRIX mInvWorld;

		D3DXMATRIX test = *pPawnA->getWorldMtx();

		D3DXMatrixInverse(&mInvWorld, nullptr, pPawnA->getWorldMtx());
		// ���C���t���[���h�ϊ�
		D3DXVECTOR3 vRayPos, vRayDir;
		D3DXVec3TransformCoord(&vRayPos, pRayPos, &mInvWorld);
		
		if (bSegment)
		{
			D3DXVec3TransformCoord(&vRayDir, pRayDir, &mInvWorld);
		}
		else
		{
			D3DXVec3TransformNormal(&vRayDir, pRayDir, &mInvWorld);
		}

		// ���C�ƃ��b�V���̌�������
		INT nIndex = Intersect(pPawnA,&vRayPos, &vRayDir, bSegment, pCross, pNormal,Length);

		if (nIndex >= 0) // �������Ă���ꍇ
		{	
			// ��_�A�@�������[���h�ϊ�
			if (pCross)
			{
				D3DXVec3TransformCoord(pCross, pCross, pPawnA->getWorldMtx());
			}
			if (pNormal)
			{
				D3DXVec3TransformNormal(pNormal, pNormal, pPawnA->getWorldMtx());
			}
		}
		return nIndex;


	return false;
}

//����������������������������������������������������������������������������
// �����̔���
//����������������������������������������������������������������������������
INT Collision::Intersect(Pawn *pField,LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal ,LPD3DXVECTOR3 pFLength)
{
	if (!pRayPos || !pRayDir)
		return -1;

	// ���C�擾
	D3DXVECTOR3& P0 = *pRayPos;
	D3DXVECTOR3 W = *pRayDir;
	
	if (bSegment)
		W -= P0;

	DWORD dwNumIndx = pField->getIndxNum();			// �O�p�`�̐��擾	
	MESH_VTX *pVtx  = pField->getVtxAcess();		// ���_���擾
	WORD	 *pIndx = pField->getIndxAcess();		// �C���f�b�N�X���擾

	// �S�O�p�`��T��
	for (DWORD i = 0; i < dwNumIndx; i += 3)
	{
		D3DXVECTOR3 P1(pVtx[pIndx[i + 0]].VtxPos);
		D3DXVECTOR3 P2(pVtx[pIndx[i + 1]].VtxPos);
		D3DXVECTOR3 P3(pVtx[pIndx[i + 2]].VtxPos);
		

		// �ӂ������x�N�g�����擾
		D3DXVECTOR3 V1(P2 - P1);
		D3DXVECTOR3 V2(P3 - P2);

		// ��O����
		if (V1.y >= pRayDir->y &&
			V2.y >= pRayDir->y)
			continue;

		// �@���x�N�g�����擾
		D3DXVECTOR3 N;
		D3DXVec3Normalize(&N, D3DXVec3Cross(&N, &V1, &V2));
		// ������Z�o
		FLOAT deno = D3DXVec3Dot(&N, &W);
		if (deno >= 0.0f)
		{
			continue;	// ���s(==0)��������\(>0)
		}

		// ���O����
		D3DXVECTOR3 N1;	
		D3DXVec3Cross(&N1, &V1, &W);
		if (D3DXVec3Dot(&N1, &(P0 - P1)) < 0.0f)
		{
			continue;
		}

		D3DXVec3Cross(&N1, &V2, &W);
		
		if (D3DXVec3Dot(&N1, &(P0 - P2)) < 0.0f)
		{
			continue;
		}

		D3DXVECTOR3 V3(P1 - P3);
		D3DXVec3Cross(&N1, &V3, &W);
		
		if (D3DXVec3Dot(&N1, &(P0 - P3)) < 0.0f)
		{
			continue;
		}

		// �}��ϐ��Z�o
		float T = D3DXVec3Dot(&N, &(P1 - P0)) / deno;
	
		pFLength->x = T;

		// ��_���Z�o
		D3DXVECTOR3 X = P0 + T * W;
		if (pCross)
			*pCross = X;

		// �@����Ԃ�
		if (pNormal)
			*pNormal = N;
		// ���������̂ŎO�p�`�̔ԍ���Ԃ�

		return i / 3;
	}
	// ������Ȃ������̂ŕ��̒l��Ԃ�
	return -1;
}

//��������������������������������������������������������������
// �����̔���
//��������������������������������������������������������������
bool Collision::IntersectA(Pawn* pField,LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld)
{
	bool ans = false;

	D3DXVECTOR3 W;
	D3DXVECTOR3 H;
	D3DXMATRIX matInv;
	if (pWorld) 
	{
		D3DXMatrixInverse(&matInv, nullptr, pWorld);
		D3DXVec3TransformCoord(&W, pRayPos, &matInv);
		D3DXVec3TransformNormal(&H, pRayDir, &matInv);
	}
	else 
	{
		W = *pRayPos;
		H = *pRayDir;
	}

	LPDIRECT3DVERTEXBUFFER9	lpVertexBuffer;	// ���_�o�b�t�@
	pField->getMesh()->GetVertexBuffer(&lpVertexBuffer);	// ���_�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
	LPDIRECT3DINDEXBUFFER9	lpIndexBuffer;	// �C���f�b�N�X�o�b�t�@
	pField->getMesh()->GetIndexBuffer(&lpIndexBuffer);		// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
	DWORD dwIdx = pField->getMesh()->GetNumFaces() * 3;		// �ʐ�����C���f�b�N�X�����Q�b�g

	MESH_VTX* pVtx;
	WORD* pIdx;
	lpVertexBuffer->Lock(0, 0, (void**)&pVtx, 0);	// ���_�o�b�t�@�����b�N
	lpIndexBuffer->Lock(0, 0, (void**)&pIdx, 0);	// �C���f�b�N�X�o�b�t�@�����b�N

	D3DXVECTOR3* P[4];
	D3DXVECTOR3 N;
	for (DWORD i = 0; i < dwIdx; ) 
	{
		// �O�p�`�̒��_�𒊏o
		P[0] = &pVtx[pIdx[i++]].VtxPos;
		P[1] = &pVtx[pIdx[i++]].VtxPos;
		P[2] = &pVtx[pIdx[i++]].VtxPos;
		P[3] = P[0];
		INT j = 0;
		for (; j < 3; j++) {
			// �O�p�`���͂ޕ��ʂ̖@���x�N�g�������߂�
			D3DXVec3Cross(&N, &H, &(*(P[j + 1]) - *(P[j])));
			// �n�_�����ʂ̕\�Ȃ瓖�����Ă��Ȃ�
			FLOAT dot = D3DXVec3Dot(&N, &(W - *(P[j])));

			if (dot > 0.0f) 
			{
				break;
			}
		}
		if (j < 3) 
		{
			continue;
		}

		// �O�p�`�̖@���x�N�g�������߂�
		D3DXVec3Cross(&N, &(*(P[1]) - *(P[0])), &(*(P[2]) - *(P[1])));
		D3DXVec3Normalize(&N, &N);

		// �}��ϐ�t�̕�������߂�
		float base = D3DXVec3Dot(&N, &H);
		if (base == 0.0f)
		{
			continue;		// ���ʂƕ��s
		}

		// t�����߂�
		FLOAT t = D3DXVec3Dot(&N, &(*(P[0]) - W)) / base;

	//	PrintDebugProc("aaaaaaa:%f\n", t);

		// ��_�����C�̌��
		if (t < 0.0f)
		{
			continue;
		}
		// ��_�����C�̑O
		if (t > 1.0f)
		{
			continue;
		}

		// ��_�����߂�
		D3DXVECTOR3 X = W + t * H;
		if (pCross) 
		{
			if (pWorld)
			{
				D3DXVec3TransformCoord(pCross, &X, pWorld);
			}
			else 
			{
				*pCross = X;
			}
		}
		if (pNormal)
		{
			if (pWorld) 
			{
				D3DXVec3TransformNormal(pNormal, &N, pWorld);
			}
			else
			{
				*pNormal = N;
			}
		}
		ans = true;
		break;
	}

	lpVertexBuffer->Unlock();		// ���_�o�b�t�@���A�����b�N
	lpIndexBuffer->Unlock();			// �C���f�b�N�X�o�b�t�@���A�����b�N
//	SAFE_RELEASE(lpVertexBuffer);	// ���_�o�b�t�@�I�u�W�F�N�g�����
//	SAFE_RELEASE(lpIndexBuffer);		// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�����

	return ans;
}

//����������������������������������������������������������������
// AABB�̂����蔻��
//����������������������������������������������������������������
bool Collision::isHitAABB(const TransformData transformA, const TransformData transformB) const
{
	// �ړ��ʂ𑫂�������v�Z����
	D3DXVECTOR3 aBoxPos = transformA.posData + transformA.velocityData;
	D3DXVECTOR3 bBoxPos = transformB.posData + transformB.velocityData;

	D3DXVECTOR3 aBoxSize = transformA.collisionBox;
	D3DXVECTOR3 bBoxSize = transformB.collisionBox;

	if (aBoxPos.x + bBoxSize.x > bBoxPos.x				 &&
		aBoxPos.x			   < bBoxPos.x + bBoxSize.x  &&
		aBoxPos.y + bBoxSize.y > bBoxPos.y				 &&
		aBoxPos.y			   < bBoxPos.y + bBoxSize.y  &&
		aBoxPos.z + bBoxSize.z > bBoxPos.z				 &&
		aBoxPos.z			   < bBoxPos.z + bBoxSize.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//����������������������������������������������������������������
// �v���C���[�ƃA�C�e���֘A�pAABB����
//����������������������������������������������������������������
bool Collision::IsHitAABBItem(Player *pPlayer,Pawn *pPawn)
{
	D3DXVECTOR3 BoxASize = pPlayer->getCollisionBox()	* 2;
	D3DXVECTOR3 BoxBSize = pPawn->getCollisionBox()		* 2;

	D3DXVECTOR3 BoxAPos = pPlayer->getOffset();
	D3DXVECTOR3 BoxBPos = pPawn->getColliderPos() + D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	if (BoxAPos.x + BoxASize.x > BoxBPos.x				 &&
		BoxAPos.x			   < BoxBPos.x + BoxBSize.x  &&
		BoxAPos.y + BoxASize.y > BoxBPos.y				 &&
		BoxAPos.y			   < BoxBPos.y + BoxBSize.y  &&
		BoxAPos.z + BoxASize.z > BoxBPos.z				 &&
		BoxAPos.z			   < BoxBPos.z + BoxBSize.z)
	{
		return true;
	}

	return false;
}

//����������������������������������������������������������������������
// �^�O�ɉ����ĕ���p (����A���Ƀv���C���[��u���d�l)
//����������������������������������������������������������������������
void Collision::SwitchHitType(Pawn *pPawnA, Pawn *pPawnB)
{
	// �v���C���[�̏���
	if (pPawnA->getTag() == Pawn::TagType::Player)
	{
		if (pPawnB->getTag() == Pawn::TagType::Enemy)
			pPawnA->setHitFlg(true);
	}
}

//��������������������������������������������������
// �擾
//��������������������������������������������������
const TransformData* Collision::getTransformData(std::string keyName,INT index)
{
	return collisionMapes[keyName].getData(index);
}

void Collision::checkPlayerCollision()
{

}