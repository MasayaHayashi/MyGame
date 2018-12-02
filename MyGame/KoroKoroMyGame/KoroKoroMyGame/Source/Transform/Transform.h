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
typedef struct 
{
	D3DXVECTOR3				posData;
	D3DXVECTOR3				velocityData;
	D3DXVECTOR3				rotDegData;
	D3DXVECTOR3				scaleData;
	D3DXVECTOR3				collisionBox;
	GameObjType				objType;
	UINT					idNumber;	// ���ʔԍ�
	bool					isHit;		
	bool					isUsed;		// �g�p����Ă��邩
} TransformData;

class Transform final
{
public :
	Transform();
	~Transform();

	void create();
	const TransformData* getData(INT index) const;
	void setData(TransformData *setData);
	void setHit(INT index,bool setFlg);
private:
	std::list< TransformData* > transformList;
};

#endif