//����������������������������������������������������������������
// C_MainField.h
// ���C���t�B�[���h�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MAINFIELD_H
#define MAINFIELD_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"

// ===== �萔�E�}�N����` =====
#define FIELD_SIZE (5300.0f)	//  2/22 5300
#define FIELD_RADIUS (FIELD_SIZE * 0.5f)
#define HIGH_POS (50.0f)	// �ő卂�x
#define LOW_POS (0.0f)		// ���n�ʒu

// ===== �N���X��` =====
class C_MAIN_FIELD : public Pawn
{
public:
	C_MAIN_FIELD();
	~C_MAIN_FIELD();
	virtual	void initializeMeshField();
	virtual	void UninitMeshField();
	virtual	void UpdateMeshField();
	virtual	void DrawMeshField();
	D3DXVECTOR3 GetCenterPos();		// ���S���W�擾
	void SetMaterialcolorEmissive(float fRed, float Green, float Blue, float Alpha);
	bool GetCollisionCiecle(D3DXVECTOR3 CurrentPos, float);
	FLOAT getCollisionRadius();

private:
};

#endif
