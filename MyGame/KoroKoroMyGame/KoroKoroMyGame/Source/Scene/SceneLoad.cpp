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

	SceneManager *pSceneManager = GetSceneManager();

	SceneManager::SceneState nextScene = SceneManager::getNextScene();

	SceneManager::SceneState NextScene = pSceneManager->GetNextScene();
	pSceneManager->SetCurrentScene(NextScene);	// ���݂̃V�[�������̃V�[���ɏ㏑��
	pSceneManager->SetScene(NextScene);

	// ���̃V�[���̏�����
	pSceneManager->InitScene();


	mutexObj.unlock();

	C_SCENE_LOAD::SetLoadFlg(true);

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
	pLight = NEW C_LIGHT;
	pLight->InitLight();

	
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void C_SCENE_LOAD::UninitScene()
{
	// �^�C�g�����S�㏈��
	pLoadIcon->UninitObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->UninitCamera();
	SAFE_DELETE(pCamera);

	pLight->UninitLight();
	SAFE_DELETE(pLight);

	pTitleObj->UninitObject();
	SAFE_DELETE(pTitleObj);
}

//������������������������������������
// �X�V
//������������������������������������
void C_SCENE_LOAD::UpdateLoad()
{
	// ���[�h�I�u�W�F�N�g�X�V
	pLoadIcon->UpdateObject();
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

	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

#if 1

	// �V�K�X���b�h���쐬
	std::thread Thred(InitNextResorce);

	// ���[�h��ʕ`��
	while (1)
	{
		dwCurrentTime = timeGetTime();

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
		{
			dwExecLastTime = dwCurrentTime;

			UpdateLoad();
			DrawScene();

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
void C_SCENE_LOAD::DrawScene()
{
	// �I�u�W�F�N�g�擾
	LPDIRECT3DDEVICE9 devicePtr = ();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	devicePtr->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(devicePtr->BeginScene()))
	{
		pLoadIcon->DrawObject();	// ���[�h

		pCamera->SetCamera();		// �J�����Z�b�g

		// Direct3D�ɂ��`��̏I��
		devicePtr->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	devicePtr->Present(NULL, NULL, NULL, NULL);
}

//��������������������������������������������������
// ���[�h�t���O�Z�b�g
//��������������������������������������������������
void C_SCENE_LOAD::SetLoadFlg(bool bSet)
{
	finishedLoad = bSet;
}