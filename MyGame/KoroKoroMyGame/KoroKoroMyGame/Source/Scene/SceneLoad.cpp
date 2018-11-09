//����������������������������������������������
// C_SceneLoad.cpp
// �V�[�����[�h
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneLoad.h"
#include "../DirectX3D/DirectX3D.h"
#include "../SceneManager/SceneManager.h"

/*
#include "C_LoadUiIcon.h"
#include "main.h"
#include "debugproc.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_TitleObj.h"
#include "C_FADE.h"
*/

// ===== �ÓI�����o =====
bool C_SCENE_LOAD::finishedLoad; // ���[�h�I������

//������������������������������������
// �R���X�g���N�^
//������������������������������������
C_SCENE_LOAD::C_SCENE_LOAD()
{
	finishedLoad = false;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
C_SCENE_LOAD::~C_SCENE_LOAD()
{

}

//������������������������������������
// �ʃX���b�h�p���[�h
//������������������������������������
void InitNextResorce()
{
	std::mutex mutexObj;
	mutexObj.lock();

	SceneManager *pSceneManager = getSceneManager();

	SceneManager::SceneState nextScene = SceneManager::getNextScene();

	SceneManager::SceneState NextScene = pSceneManager->getNextScene();
	pSceneManager->setCurrentScene(NextScene);	// ���݂̃V�[�������̃V�[���ɏ㏑��
	pSceneManager->setScene(NextScene);

	// ���̃V�[���̏�����
	pSceneManager->InitScene();


	mutexObj.unlock();

	C_SCENE_LOAD::setLoadFlg(true);

}

//����������������������������������
// ������
//����������������������������������
void C_SCENE_LOAD::InitScene()
{
	// ���[�h�A�C�R��������
	pLoadIcon = NEW C_LOAD_UI_ICON;
	pLoadIcon->InitObject();

	// �J����������
	pCamera = NEW C_CAMERA;
	pCamera->InitCamera();

	// ���C�g������
	pLight = NEW Light;
	pLight->InitLight();

	
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void C_SCENE_LOAD::finalizeScene()
{
	// �^�C�g�����S�㏈��
	pLoadIcon->finalizeObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	pTitleObj->finalizeObject();
	SAFE_DELETE(pTitleObj);
}

//������������������������������������
// �X�V
//������������������������������������
void C_SCENE_LOAD::updateLoad()
{
	// ���[�h�I�u�W�F�N�g�X�V
	pLoadIcon->updateObject();
}


//������������������������������������
// ���[�h�J�n
//������������������������������������
void C_SCENE_LOAD::EnableLoad()
{
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	dwExecLastTime = dwFPSLastTime = timegetTime();
	dwCurrentTime = dwFrameCount = 0;

#if 1

	// �V�K�X���b�h���쐬
	std::thread Thred(InitNextResorce);

	// ���[�h��ʕ`��
	while (1)
	{
		dwCurrentTime = timegetTime();

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
		{
			dwExecLastTime = dwCurrentTime;

			updateLoad();
			drawScene();

			// ���[�h����
			if (finishedLoad)
				break;
		}
	}
	Thred.join();

	finishedLoad = false;
#endif
}

//��������������������������������������
// �`��
//��������������������������������������
void C_SCENE_LOAD::drawScene()
{
	// �I�u�W�F�N�g�擾
	LPDIRECT3DDEVICE9 devicePtr = ();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	devicePtr->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(devicePtr->BeginScene()))
	{
		pLoadIcon->drawObject();	// ���[�h

		pCamera->setCamera();		// �J�����Z�b�g

		// Direct3D�ɂ��`��̏I��
		devicePtr->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	devicePtr->Present(nullptr, nullptr, nullptr, nullptr);
}

//��������������������������������������������������
// ���[�h�t���O�Z�b�g
//��������������������������������������������������
void C_SCENE_LOAD::setLoadFlg(bool bset)
{
	finishedLoad = bset;
}