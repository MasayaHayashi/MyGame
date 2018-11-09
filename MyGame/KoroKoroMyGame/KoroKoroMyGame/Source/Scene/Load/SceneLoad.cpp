//����������������������������������������������
// C_SceneLoad.cpp
// �V�[�����[�h
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneLoad.h"
#include "C_SceneManager.h"

/*
#include "C_LoadUiIcon.h"
#include "main.h"
#include "debugproc.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_TitleObj.h"
#include "C_FADE.h"
*/

// ===== �O���[�o���ϐ��錾 =====
std::mutex *pMutex;
bool C_SCENE_LOAD::bFinishedLoad; // ���[�h�I������

//������������������������������������
// �R���X�g���N�^
//������������������������������������
C_SCENE_LOAD::C_SCENE_LOAD()
{
	pMutex = NEW std::mutex;

	bFinishedLoad = false;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
C_SCENE_LOAD::~C_SCENE_LOAD()
{
	SAFE_DELETE(pMutex)
}

//������������������������������������
// �ʃX���b�h�p���[�h
//������������������������������������
void InitNextResorce()
{
	pMutex->lock();

	C_SCENE_MANAGER *pSceneManager = GetSceneManager();

	C_SCENE_MANAGER::SCENE_STATE NextScene = pSceneManager->GetNextScene();
	pSceneManager->SetCurrentScene(NextScene);	// ���݂̃V�[�������̃V�[���ɏ㏑��
	pSceneManager->SetScene(NextScene);

	// ���̃V�[���̏�����
	pSceneManager->InitScene();


	pMutex->unlock();

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
			if (bFinishedLoad)
				break;
		}
	}
	Thred.join();

	bFinishedLoad = false;
#endif
}

//��������������������������������������
// �`��
//��������������������������������������
void C_SCENE_LOAD::DrawScene()
{
	// �I�u�W�F�N�g�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		pLoadIcon->DrawObject();	// ���[�h

		pCamera->SetCamera();		// �J�����Z�b�g

		// Direct3D�ɂ��`��̏I��
		pDevice->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//��������������������������������������������������
// ���[�h�t���O�Z�b�g
//��������������������������������������������������
void C_SCENE_LOAD::SetLoadFlg(bool bSet)
{
	bFinishedLoad = bSet;
}