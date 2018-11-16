//����������������������������������������������������
// C_SceneBase.h
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef C_SCENEBASE_H
#define C_SCENEBASE_H

// ===== �N���X�̑O���錾 =====
class Camera;

// ===== �C���N���[�h�� =====
#include "../Audio/\audiere.h"
#include "d3dx9.h"


// ===== �N���X��` =====
class C_SCENE_BASE
{
public:
	C_SCENE_BASE();
	virtual ~C_SCENE_BASE();

	virtual void initialize()   = 0;
	virtual void finalize() = 0;
	virtual void update() = 0;
	virtual void draw()   = 0;

	virtual void initializeStatus()  = 0;		// �e��X�e�[�^�X���Z�b�g

	// �Q�b�^�[�֘A
	virtual Camera* getCamera() = 0;

protected:
	audiere::OutputStreamPtr bgmPtr;
	audiere::SoundEffectPtr	 sePtr;
private:
};

#endif