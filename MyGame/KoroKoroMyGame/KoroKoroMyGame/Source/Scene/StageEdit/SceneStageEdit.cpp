//����������������������������������������������
// SceneStageEdit.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneStageEdit.h"
#include "../../DirectX3D/DirectX3D.h"
#include "../../Skydome/Skydome.h"

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneStageEdit::SceneStageEdit()
{
	skydomePtr.reset(NEW Skydome());
	stageEditorPtr.reset(NEW StageEditor());
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneStageEdit::~SceneStageEdit()
{

}

//����������������������������������
// ������
//����������������������������������
void SceneStageEdit::initialize()
{
	skydomePtr->initialize();
	stageEditorPtr->initialize();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void SceneStageEdit::finalize()
{
	skydomePtr->finalize();
	stageEditorPtr->finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void SceneStageEdit::update()
{
	skydomePtr->update();
	stageEditorPtr->update();
}

//��������������������������������������
// �`��
//��������������������������������������
void SceneStageEdit::draw()
{
	stageEditorPtr->draw();
	skydomePtr->draw();
}


//����������������������������������
// �J�����擾
//����������������������������������
Camera* SceneStageEdit::getCamera()
{
	return stageEditorPtr->getCamera();
}

//������������������������������������
// �X�e�[�^�X������
//������������������������������������
void SceneStageEdit::initializeStatus()
{
	
}