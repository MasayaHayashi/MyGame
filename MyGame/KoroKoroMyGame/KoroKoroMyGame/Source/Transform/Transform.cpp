//����������������������������������������������������������������
// Transform.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Transform.h"
#include "../DirectX3D/DirectX3D.h"
#include <memory>

//��������������������������
// �R���X�g���N�^
//��������������������������
Transform::Transform()
{

}

//��������������������������
// �f�X�g���N�^
//��������������������������
Transform::~Transform()
{

}

//��������������������������
// ����
//��������������������������
void Transform::create()
{

}

//��������������������������
// �f�[�^�擾
//��������������������������
const TransformData* Transform::getData() const
{
	auto itr = transformList.begin();

	return nullptr;
}

//��������������������������
// �f�[�^�Z�b�g
//��������������������������
void Transform::setData(TransformData setData)
{
	transformList.push_back(new TransformData);
}