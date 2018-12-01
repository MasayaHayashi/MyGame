//����������������������������������������������������������������
// Transform.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef TRANSFORM_H
#define TRANSFORM_H

// ===== �C���N���[�h�� =====
#include <list>
#include <memory>
#include "d3dx9.h"
#include "../GameObjectBase/GameObjectBase.h"

// ===== �\���̒�` =====
class TransformData final
{
public :
	D3DXVECTOR3				posData;	// ���W
	D3DXVECTOR3				rotDegData;
	D3DXVECTOR3				scaleData;	// �g��
	//GameObjType				objType;	// �ǂ̃I�u�W�F�N�g��
	UINT					idNumber;	// ���ʔԍ�
	bool					isUsed;		// �g�p����Ă��邩
};

class Transform final
{
public :
	Transform();
	~Transform();

	void create();
	const TransformData* getData() const;
	void setData(TransformData setData);
private:
	std::list< std::unique_ptr<TransformData> > transformList;
};

#endif