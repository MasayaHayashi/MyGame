//����������������������������������������������
// C_SceneLoad.cpp
// �V�[�����[�h
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneLoad.h"
#include "../DirectX3D/DirectX3D.h"
#include "../SceneManager/SceneManager.h"
#include "Load\SceneLoad.h"

/*
#include "C_LoadUiIcon.h"
#include "main.h"
#include "debugproc.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_TitleObj.h"
#include "FadeUI.h"
*/

// ===== �ÓI�����o =====
bool SceneLoad::finishedLoad;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneLoad::SceneLoad()
{
	finishedLoad = false;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneLoad::~SceneLoad()
{

}

//������������������������������������
// �ʃX���b�h�p���[�h
//������������������������������������
void initializeNextResorce()
{
	std::mutex mutexObj;
	mutexObj.lock();

	SceneManager::SceneState nextScene = SceneManager::getNextScene(); SceneManager::getNextScene();

	SceneManager::SceneState NextScene = SceneManager::getNextScene();
	SceneManager::setCurrentScene(NextScene);	// ���݂̃V�[�������̃V�[���ɏ㏑��
	SceneManager::changeScene(NextScene);

	// ���̃V�[���̏�����
	SceneManager::initialize();


	mutexObj.unlock();

	SceneLoad::setLoadFlg(true);

}

//����������������������������������
// ������
//����������������������������������
void SceneLoad::initialize()
{
	/*
	// ���[�h�A�C�R��������
	pLoadIcon = NEW C_LOAD_UI_ICON;
	pLoadIcon->initialize();

	// �J����������
	pCamera = NEW C_CAMERA;
	pCamera->initializeCamera();

	// ���C�g������
	pLight = NEW Light;
	pLight->initializeLight();

	
	pTitleObj = NEW HeartObj;
	pTitleObj->initialize();
	*/
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void SceneLoad::finalize()
{
	/*
	// �^�C�g�����S�㏈��
	pLoadIcon->finalizeObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	pTitleObj->finalizeObject();
	SAFE_DELETE(pTitleObj);
	*/
}

//������������������������������������
// �X�V
//������������������������������������
void SceneLoad::update()
{
	// ���[�h�I�u�W�F�N�g�X�V
//	pLoadIcon->updateObject();
}


//������������������������������������
// ���[�h�J�n
//������������������������������������
void SceneLoad::enable()
{
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

#if 1

	// �V�K�X���b�h���쐬
	std::thread Thred(initializeNextResorce);

	// ���[�h��ʕ`��
	while (1)
	{
		dwCurrentTime = timeGetTime();

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
		{
			dwExecLastTime = dwCurrentTime;

			update();
			draw();

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
void SceneLoad::draw()
{
	// �I�u�W�F�N�g�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	devicePtr->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(devicePtr->BeginScene()))
	{
		//pLoadIcon->drawObject();	// ���[�h

		//pCamera->setCamera();		// �J�����Z�b�g

		// Direct3D�ɂ��`��̏I��
		devicePtr->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	devicePtr->Present(nullptr, nullptr, nullptr, nullptr);
}

//��������������������������������������������������
// ���[�h�t���O�Z�b�g
//��������������������������������������������������
void SceneLoad::setLoadFlg(bool bset)
{
	finishedLoad = bset;
}