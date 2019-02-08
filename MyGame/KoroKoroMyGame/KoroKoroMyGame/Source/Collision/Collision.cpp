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
#include "../SceneManager/SceneManager.h"
#include "../MyVector3/MyVector3.h"
#include "../GameManager/GameManager.h"
#include "../MyDelete/MyDelete.h"

// ===== �ÓI�����o =====
std::unordered_map<std::string, std::list<		Transform*		 >>	Collision::collisionMapes;
std::unordered_map<std::string, std::vector<	RayHit*>		 >	Collision::rayHitMapes;
std::unordered_map<std::string, std::list<		CameraTransform* >>	Collision::cameraTransforms;
D3DXVECTOR3 Collision::cross;
INT Collision::hitIndex = 0;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
Collision::Collision()
{
	
}

//������������������������������������
// �R���X�g���N�^
//������������������������������������
Collision::Collision(Pawn* setPlayerPtr,Pawn* setFieldPtr)
{
	playersPtr.push_back(setPlayerPtr);
	fieldPtres.push_back(setFieldPtr);
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
	D3DXVECTOR3 normal, length;

	UINT playerIndex = 0;
	
	checkCollision("Player", "heart.x");

	checkCollisionBlock();

	checkCollision("Player","star.x");

	for (auto fieldPtr : fieldPtres)
	{
		if (!fieldPtr->getUsedFlg())
		{
			continue;
		}

		if (!fieldPtr->getIsFieldObject())
		{
			continue;
		}

		if (checkCollisionField(playersPtr.front(), playersPtr.front(), fieldPtr, cross, normal, rayHitMapes["Player"].front()->length, D3DXVECTOR3(0.0f, -1.0f, 0.0f)))
		{
			if (std::abs(rayHitMapes["Player"].front()->length) < 1.0f)
			{
				rayHitMapes["Player"][playerIndex]->isHit = true;
				return;
			}
			else
			{
				rayHitMapes["Player"][playerIndex]->isHit = false;
			}
		}
		else
		{
			rayHitMapes["Player"][playerIndex]->isHit = false;
		}
	}
	
}

//������������������������������������
// �o�^
//������������������������������������
void Collision::registerList(Transform *setPawn,std::string keyName)
{
	collisionMapes[keyName].push_back( setPawn);

	rayHitMapes[keyName].push_back(NEW RayHit());
}

//����������������������������������
// �o�^
//����������������������������������
void Collision::registerList(CameraTransform *setCamera, std::string keyName)
{
	cameraTransforms[keyName].push_back( setCamera);
}

//����������������������������������������������
// ���
//����������������������������������������������
void Collision::release(std::string keyName)
{
	if (collisionMapes[keyName].empty())
	{
		return;
	}

	for (size_t playerIndex = 0; playerIndex < collisionMapes[keyName].size(); playerIndex)
	{
		collisionMapes[keyName].pop_back();
	}
//	collisionMapes[keyName].clear();
}

//������������������������������������
// �㏈��
//������������������������������������
void Collision::finalize(std::string keyName)
{
	for (auto & rayHitMap : rayHitMapes)
	{
		for (auto &vector : rayHitMap.second)
		{
			Mydelete::safeDelete(vector);
		}
	}

	for (UINT playerIndex = 0; playerIndex < collisionMapes[keyName].size(); playerIndex++)
	{
		Mydelete::safeDelete(collisionMapes[keyName].back());
		collisionMapes[keyName].pop_back();
	}

	collisionMapes[keyName].clear();

	for (UINT playerIndex = 0; playerIndex < rayHitMapes[keyName].size(); playerIndex++)
	{
		Mydelete::safeDelete(rayHitMapes[keyName].back());
		rayHitMapes[keyName].pop_back();
	}

	/*
	for (UINT playerCnt = 0; playerCnt < rayHitMapes["Player"].size(); playerCnt++)
	{
		Mydelete::safeDelete(rayHitMapes["Player"].back());
		rayHitMapes["Player"].pop_back();
	}

	for (UINT fieldCnt = 0; fieldCnt < rayHitMapes["field"].size(); fieldCnt++)
	{
		delete rayHitMapes["field"].back();
		rayHitMapes["field"].pop_back();
	}
	*/
	
}

//������������������������������������
// �n�ʂƃv���C���[�̏Փˏ����X�V
//������������������������������������
UINT Collision::checkCollisionField(Pawn *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, FLOAT &length, D3DXVECTOR3 DestVec)
{
	INT	nIndx;

	// �����ƎO�p�`�̔���
	nIndx = isHitRayToMesh(pField, pPlayer, &pPlayer->getPosition(), &(pPlayer->getPosition() + DestVec), true, &Cross, &Normal, length);

	if (nIndx >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
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

//��������������������������������������������
// �v���C���[�Z�b�g
//��������������������������������������������
void Collision::registerPlayer(Pawn* playerPtr)
{
	playersPtr.push_back(playerPtr);
}

//��������������������������������������������
// �t�B�[���h�Z�b�g
//��������������������������������������������
void Collision::registerField(Pawn* setFieldPtr)
{
	fieldPtres.push_back(setFieldPtr);
}

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
INT Collision::isHitRayToMesh(Pawn *pPawnA, Pawn *pPawnB, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, FLOAT &length)
{
	// ���[���h�}�g���b�N�X�̋t�}�g���b�N�X�𐶐�
	D3DXMATRIX mInvWorld;

	D3DXMatrixInverse(&mInvWorld, NULL, pPawnA->getWorldMtx());
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
	INT nIndex = Intersect(pPawnA, &vRayPos, &vRayDir, bSegment, pCross, pNormal, length);

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
}

//����������������������������������������������������������������
// AABB�̂����蔻��
//����������������������������������������������������������������
const bool Collision::isHitAABB(Transform pPawnA, Transform pPawnB)
{
	if (pPawnA.pos.x - pPawnA.collisionBox.x < pPawnB.pos.x	+ pPawnB.collisionBox.x &&
		pPawnA.pos.x + pPawnA.collisionBox.x > pPawnB.pos.x - pPawnB.collisionBox.x &&
		pPawnA.pos.y + pPawnA.collisionBox.y > pPawnB.pos.y				 &&
		pPawnA.pos.y < pPawnB.pos.y + pPawnB.collisionBox.y * 2 &&
		pPawnA.pos.z + pPawnA.collisionBox.z > pPawnB.pos.z				 &&
		pPawnA.pos.z  < pPawnB.pos.z + pPawnB.collisionBox.z * 2)
	{
		return true;
	}

	return false;
}

//����������������������������������������������������������������
// AABB�̂����蔻��
//����������������������������������������������������������������
const bool Collision::isHitAABB(Transform pPawnA, Pawn &pPawnB)
{
	if (pPawnA.pos.x - pPawnA.collisionBox.x < pPawnB.getPosition().x + pPawnB.getCollisionBox().x * 0.75f &&
		pPawnA.pos.x + pPawnA.collisionBox.x > pPawnB.getPosition().x - pPawnB.getCollisionBox().x * 0.75f &&
		pPawnA.pos.y - pPawnA.collisionBox.y < pPawnB.getPosition().y + pPawnB.getCollisionBox().y * 2.5f &&
		pPawnA.pos.y + pPawnA.collisionBox.y > pPawnB.getPosition().y - pPawnB.getCollisionBox().y * 2.5f &&
		pPawnA.pos.z - pPawnA.collisionBox.z < pPawnB.getPosition().z + pPawnB.getCollisionBox().z * 0.9f  &&
		pPawnA.pos.z + pPawnA.collisionBox.z > pPawnB.getPosition().z - pPawnB.getCollisionBox().z * 0.9f)
	{
		return true;
	}

	return false;
}

//����������������������������������������������������������������������������
// �����̔���
//����������������������������������������������������������������������������
INT Collision::Intersect(Pawn *pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, FLOAT &length)
{
	if (!pRayPos || !pRayDir)
	{
		return -1;
	}

	// ���C�擾
	D3DXVECTOR3& P0 = *pRayPos;
	D3DXVECTOR3 W = *pRayDir;

	if (bSegment)
	{
		W -= P0;
	}

	DWORD dwNumIndx = pField->getIndxNum();			// �O�p�`�̐��擾	
	MESH_VTX *pVtx = pField->getVtxAcess();		// ���_���擾
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
		{
			continue;
		}

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

		length = T;

		// ��_���Z�o
		D3DXVECTOR3 X = P0 + T * W;

		if (pCross)
		{
			*pCross = X;
		}

		// �@����Ԃ�
		if (pNormal)
		{
			*pNormal = N;
		}
		// ���������̂ŎO�p�`�̔ԍ���Ԃ�

		return i / 3;
	}
	// ������Ȃ������̂ŕ��̒l��Ԃ�
	return -1;
}

//��������������������������������������������������
// �擾
//��������������������������������������������������
const Transform* Collision::getTransform(std::string keyName,INT index)
{
	if (index < 0)
	{
		throw std::invalid_argument("�s���Ȉ����ł�");

		return nullptr;
	}

	return *std::next(collisionMapes[keyName].begin(), index);
}

//��������������������������������������������������
// �擾
//��������������������������������������������������
const CameraTransform* Collision::getCameraTransform(std::string keyName, INT index)
{
	if (index < 0)
	{
		throw std::invalid_argument("�s���Ȉ����ł�");
	}
	
	return *std::next(cameraTransforms[keyName].begin(), index);
}

//������������������������������������������������������������������������
// ���C�̔���擾
//������������������������������������������������������������������������
const RayHit* Collision::getRayHitData(std::string keyName,UINT index)
{
	if (index < 0)
	{
		throw std::underflow_error("�����̒l���}�C�i�X�ł�");
	}

	return *std::next(rayHitMapes[keyName].begin(), index);
}

//������������������������������������������������
// �����x�Z�b�g
//������������������������������������������������
void Collision::setVelocity(std::string keyName, UINT index, D3DXVECTOR3 setVelocity)
{
	UINT indexCnt = 0;

	for (auto &collisionMap : collisionMapes[keyName])
	{
		if (indexCnt == index)
		{
			collisionMap->velocity = setVelocity;
		}

		indexCnt++;
	}
}

//������������������������������������������������
// �T�C�Y�擾
//������������������������������������������������
const size_t Collision::getSize(std::string keyName)
{
	return collisionMapes[keyName].size();
}

//������������������������������������������������
// ��_�擾
//������������������������������������������������
const D3DXVECTOR3 Collision::getCross()
{
	return cross;
}

//������������������������������������������������
// �u���b�N�p
//������������������������������������������������
void Collision::registerBlock(Pawn &blockPtr)
{
	blockPtres.push_back(&blockPtr);
}

//������������������������������������������������
//�@�u���b�N�p�����蔻��擾
//������������������������������������������������
void Collision::checkCollisionBlock()
{
	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	size_t blockIndex = 0;

	for (auto &blockPtr : blockPtres)
	{
		if (!blockPtr->getUsedFlg() || 
			blockPtr->getTag() == "star.x")
		{
			continue;
		}

		auto blockItr = std::next(collisionMapes[blockPtr->getTag()].begin(), blockIndex);

		if (isHitAABB(*collisionMapes["Player"].front(), *blockPtr))
		{
			(*blockItr)->isHitAABB = true;
		}
		else
		{
			(*blockItr)->isHitAABB = false;
		}

		blockIndex++;
	}
}

//������������������������������������������������������
// AABB����
//������������������������������������������������������
void Collision::checkCollision(std::string keyNameA,std::string keyNameB)
{
	for (auto &collisionMap : collisionMapes[keyNameB])
	{
		if (!collisionMap->isUsed)
		{
			continue;
		}

		if (isHitAABB(*collisionMapes[keyNameA].front(), *collisionMap))
		{
			collisionMap->isHitAABB = true;
		}
		else
		{
			collisionMap->isHitAABB = false;
		}
	}
}

//������������������������������������������������������
// �������Ă���I�u�W�F�N�g�̃C���f�b�N�X�擾
// ���݂͕����̔���ɂ͖��Ή�
//������������������������������������������������������
const INT Collision::getHitIndex(std::string keyName)
{
	hitIndex = 0;

	for (auto & collisionMap : collisionMapes[keyName])
	{
		if (!collisionMap->isUsed)
		{
			continue;
		}

		if (collisionMap->isHitAABB)
		{
			return hitIndex;
		}
		else
		{
			hitIndex++;
		}
	}

	return -1;
}

//������������������������������������������������������
// �X�e�[�^�X������
//������������������������������������������������������
void Collision::initilize(std::string keyName,size_t index)
{
	const auto itr = std::next(rayHitMapes[keyName].begin(), index);
	(*itr)->cross = D3DXVECTOR3(0.0f, 0.0f, 5.0f);
	(*itr)->isHit = false;
}

//������������������������������������������������������
// �X�e�[�^�X������
//������������������������������������������������������
void Collision::initilize()
{
	for (auto &collisionMap : collisionMapes)
	{
		for (auto & second : collisionMap.second)
		{
			second->isHitAABB = false;
		}
	}
}