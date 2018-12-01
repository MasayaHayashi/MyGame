//����������������������������������������������������������������
// MyVector3.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "MyVector3.h"
#include <stdio.h>
#include <math.h>

//������������������������������������
// �R���X�g���N�^
//������������������������������������
MyVector3::MyVector3()
{

}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
MyVector3::~MyVector3()
{

}

//������������������������������������������������
//		���`�⊮�֐�(linear interpolate)
//	�����F
//		���ʏo�͗p
//		�J�n�l
//		�I���_�l
//		�ŏ�����
//		�ő厞��
//		���ݎ���
//��������������������������������������������������
void MyVector3::Lerp(D3DXVECTOR3* pOut, const D3DXVECTOR3& fStart, const D3DXVECTOR3& fEnd, const FLOAT& fMin, const FLOAT& fMax, const FLOAT& fCurrent)
{
	pOut->x = fStart.x + (fEnd.x - fStart.x) * (fCurrent - fMin) / (fMax - fMin);
	pOut->y = fStart.y + (fEnd.y - fStart.y) * (fCurrent - fMin) / (fMax - fMin);
	pOut->z = fStart.z + (fEnd.z - fStart.z) * (fCurrent - fMin) / (fMax - fMin);
}


//��������������������������������������������������������������������������������������
//		���ʐ��`�⊮�֐�(spherical linear interpolation)
//	�����F
//		���ʏo�͗p�N�E�H�[�^�j�I��
//		�J�n�N�E�H�[�^�j�I��
//		�I���N�E�H�[�^�j�I��
//		�ő厞��
//		���ݎ���
//��������������������������������������������������������������������������������������
void MyVector3::Slerp(D3DXQUATERNION *out, const D3DXQUATERNION &start, const D3DXQUATERNION &end, const FLOAT& fMax, const FLOAT& fCurrent)
{
	FLOAT dot_val;
	FLOAT sin_val;
	FLOAT theta;
	FLOAT scale0, scale1;
	FLOAT t = fCurrent / fMax;

	// ���ς��v�Z
	dot_val = start.x * end.x +
		start.y * end.y +
		start.z * end.z +
		start.w * end.w; // �J�n�_�ƏI���_�̓���

	// ��Ԓl���v�Z
	theta = acos(start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w);
	sin_val = sinf(theta);
	scale0 = sinf(theta * (1 - t)) / sinf(theta);
	scale1 = sinf(theta * t) / sinf(theta);	
	out->x = start.x * scale0 + end.x * scale1;
	out->y = start.y * scale0 + end.y * scale1;
	out->z = start.z * scale0 + end.z * scale1;
	out->w = start.w * scale0 + end.w * scale1;
}


//����������������������������������������������������������������������������������������������������������������������������
//	3���x�W�G�Ȑ�
//	����
//		���ʏo�͗pX���W
//		���ʏo�͗pY���W
//		�J�n�_(����_1)X���W
//		�J�n�_(����_1)Y���W
//		����_2 X���W
//		����_2 Y���W �Q�[������w��
//		����_3 X���W
//		����_3 Y���W
//		�I���_(����_4) X���W
//		�I���_(����_4) Y���W
//		�ő厞��
//		���ݎ���
//������������������������������������������������������������������������������������������������������������������������������
void MyVector3::CalcBezierCurve(D3DXVECTOR3* pOut,const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3, const D3DXVECTOR3& Pos4, const FLOAT& fMax, const FLOAT& fCurrent)
{
	float fRate = fCurrent / fMax;

	pOut->x = ((1 - fRate) * (1 - fRate) * (1 - fRate)) * Pos1.x + 3 * ((1 - fRate) * (1 - fRate)) * fRate * Pos2.x + 3 * (1 - fRate) * (fRate * fRate) * Pos3.x + (fRate * fRate * fRate) * Pos4.x;
	pOut->y = ((1 - fRate) * (1 - fRate) * (1 - fRate)) * Pos1.y + 3 * ((1 - fRate) * (1 - fRate)) * fRate * Pos2.y + 3 * (1 - fRate) * (fRate * fRate) * Pos3.y + (fRate * fRate * fRate) * Pos4.y;
	pOut->z = ((1 - fRate) * (1 - fRate) * (1 - fRate)) * Pos1.z + 3 * ((1 - fRate) * (1 - fRate)) * fRate * Pos2.z + 3 * (1 - fRate) * (fRate * fRate) * Pos3.z + (fRate * fRate * fRate) * Pos4.z;
}

//������������������������������������������������������������������������������������������������������������������������������
//
//		�G���~�[�g�Ȑ��v�Z(3��)
//
//	�����F
//		���ʏo�͗p�x�N�g��
//		�J�n�_(����_1)X,Y,Z��jy��
//		�I���_(����_2)X���W
//		�I���_(����_2)Y���W
//		�x�N�g��1 X����
//		�x�N�g��1 Y����
//		�x�N�g��2 X����
//		�x�N�g��2 Y����
//		�ő厞��
//		���ݎ���
//	�߂�l�F
//			�I�������ꍇtrue��Ԃ�
//������������������������������������������������������������������������������������������������������������������������������
bool MyVector3::CalcHermiteCurve(D3DXVECTOR3 *pOut, const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Vec1, const D3DXVECTOR3& Vec2, const FLOAT& fMax, const FLOAT& fCurrent)
{
	float fRate = fCurrent / fMax;

	pOut->x = ((fRate - 1) * (fRate - 1)) * (2 * fRate + 1) * Pos1.x + (fRate * fRate) * (3 - 2 * fRate) * Pos2.x + ((1 - fRate) * (1 - fRate)) * fRate * Vec1.x + (fRate - 1) * (fRate * fRate) * Vec2.x;
	pOut->y = ((fRate - 1) * (fRate - 1)) * (2 * fRate + 1) * Pos1.y + (fRate * fRate) * (3 - 2 * fRate) * Pos2.y + ((1 - fRate) * (1 - fRate)) * fRate * Vec1.y + (fRate - 1) * (fRate * fRate) * Vec2.y;
	pOut->z = ((fRate - 1) * (fRate - 1)) * (2 * fRate + 1) * Pos1.z + (fRate * fRate) * (3 - 2 * fRate) * Pos2.z + ((1 - fRate) * (1 - fRate)) * fRate * Vec1.z + (fRate - 1) * (fRate * fRate) * Vec2.z;

	if (fMax <= fCurrent)
		return false;

	return true;
}

//����������������������������������������������������������
// �P�ʍs�񐶐�
//����������������������������������������������������������
D3DXMATRIX* MyVector3::Mtx2DIdentity(D3DXMATRIX* pOut)
{
	return D3DXMatrixIdentity(pOut);
}

//��������������������������������������������������������
// �g��k��
//��������������������������������������������������������
D3DXMATRIX* MyVector3::Mtx2DScaling(D3DXMATRIX* pOut, FLOAT Sx, FLOAT Sy)
{
	if (pOut) 
	{
		Mtx2DIdentity(pOut);
		pOut->_11 = Sx;
		pOut->_22 = Sy;
	}
	return pOut;
}

//��������������������������������������������������������
// ���s�ړ�
//��������������������������������������������������������
D3DXMATRIX* MyVector3::Mtx2DTranslation(D3DXMATRIX* pOut, FLOAT x, FLOAT y)
{
	if (pOut)
	{
		Mtx2DIdentity(pOut);
		pOut->_31 = x;
		pOut->_32 = y;
	}
	return pOut;
}

//������������������������������������������������
// ��]
//������������������������������������������������
D3DXMATRIX* MyVector3::Mtx2DRotation(D3DXMATRIX* pOut, FLOAT Angle)
{
	if (pOut) 
	{
		Mtx2DIdentity(pOut);
		pOut->_11 = cosf(Angle);
		pOut->_22 = pOut->_11;
		pOut->_12 = sinf(Angle);
		pOut->_21 = -pOut->_12;
	}
	return pOut;
}

//����������������������������������������������
// �x�N�g���̓��όv�Z
//����������������������������������������������
FLOAT MyVector3::Vec3Dot(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB)
{
	FLOAT  Dot = (VecA.x * VecB.x) + (VecA.y * VecB.y) + (VecA.z * VecB.z);
	return Dot;
}

//����������������������������������������������
// �x�N�g���̊O��(3����)
//����������������������������������������������
D3DXVECTOR3 MyVector3::Vec3Cross(D3DXVECTOR3 VecA,D3DXVECTOR3 VecB)
{
	D3DXVECTOR3 Cross = D3DXVECTOR3((VecA.y * VecB.z) - (VecA.z * VecB.y),
									(VecA.z * VecB.x) + (VecA.x * VecB.z),
									(VecA.x * VecB.y) + (VecA.y * VecB.x));
	return Cross;
}

/*
//����������������������������������������������
// ����
//����������������������������������������������
const T CalcInnerProduct(const TNhVector2& b) const
{
	return x * b.x + y * b.y;
}

// �O��
const T CalcOuterProduct(const TNhVector2& b) const
{
	return x * b.y - y * b.x;
}
*/

// ????
FLOAT MyVector3::Product2Vec(D3DXVECTOR3 VecA,D3DXVECTOR3 VecB)
{
//	return VecA.x * VecB.y - VecA.y * VecB.x;
	return VecA.x * VecB.z - VecA.z * VecB.x;
}


//����������������������������������������������������������
// �x�N�g���ƃx�N�g���̊p�x�����߂�(���W�A��)
//����������������������������������������������������������
FLOAT MyVector3::CalcAngle(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB)
{
	FLOAT fLengthA = D3DXVec3Length(&vectorA);
	FLOAT fLengthB = D3DXVec3Length(&vectorB);

	D3DXVec3Normalize(&vectorA, &vectorA);
	D3DXVec3Normalize(&vectorB, &vectorB);
	

	FLOAT fCosAlpha = D3DXVec3Dot(&vectorA, &vectorB);

	FLOAT  fAngleRad;
	if (fCosAlpha > 1.0f)
	{
		fCosAlpha = 1.0f;
	}
	if (fCosAlpha < -1.0f)
	{
		fCosAlpha = -1.0f;
	}

	fAngleRad = acos(fCosAlpha);

	return fAngleRad;
}

//����������������������������������������������������������������������������������������������
// 0�`2�΁F�x�N�g���a���x�N�g���`�ɑ΂��Ď��v�������������Ă���Ƃ��΁`2�΂̒l���Ԃ�܂��B
//����������������������������������������������������������������������������������������������
FLOAT MyVector3::CalcAngleDegree(D3DXVECTOR3 VectorA, D3DXVECTOR3 VectorB)
{
	// ��O����
	if (VectorA.x == 0.0f &&
		VectorA.y == 0.0f &&
		VectorA.z == 0.0f
		)
		return 0.0f;

	FLOAT fAngleRad = CalcAngle(VectorA,VectorB);


	/*
	if (!CalcAngle(vectorA, vectorB, dAngle_rad))
	{
		return false;
	}
	*/
	FLOAT Profuct = Product2Vec(VectorA, VectorB);
	if (Profuct < 0.0f)
	{
		fAngleRad = 2 * 3.14 - fAngleRad;
	}

	return fAngleRad;
}