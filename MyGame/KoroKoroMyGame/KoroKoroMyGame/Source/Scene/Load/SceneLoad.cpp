//����������������������������������������������
// C_SceneLoad.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneLoad.h"
#include "../../SceneManager/SceneManager.h"
#include "../../DirectX3D/DirectX3D.h"
#include "../../Camera/Camera.h"
#include "../../Light/Light.h"
#include "../../LoadIcon/MyLoadIcon.h"

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
bool SceneLoad::isFinishedLoad; // ���[�h�I������

std::unique_ptr<Camera>			SceneLoad::cameraPtr;
std::unique_ptr<Light>			SceneLoad::lightPtr;
std::unique_ptr<MyLoadIcon>		SceneLoad::myLoadIConPtr;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneLoad::SceneLoad()
{
	pMutex = NEW std::mutex;
	cameraPtr.reset(NEW Camera());
	lightPtr.reset(NEW Light());
	myLoadIConPtr.reset(NEW MyLoadIcon());

	isFinishedLoad = false;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneLoad::~SceneLoad()
{
	delete pMutex;
	pMutex = nullptr;
}

//������������������������������������
// �ʃX���b�h�p���[�h
//������������������������������������
void InitNextResorce()
{
	pMutex->lock();

	SceneManager::SceneState nextScene = SceneManager::getNextScene();

//	C_SCENE_MANAGER::SCENE_STATE NextScene = pSceneManager->GetNextScene();

	SceneManager::setCurrentScene(nextScene);
	SceneManager::changeScene(nextScene);

//	pSceneManager->SetCurrentScene(NextScene);	// ���݂̃V�[�������̃V�[���ɏ㏑��
//	pSceneManager->SetScene(NextScene);

	SceneManager::getInstanse()->initialize();

	pMutex->unlock();

	SceneLoad::setLoadFlg(true);

}

//����������������������������������
// ������
//����������������������������������
void SceneLoad::initialize()
{
	cameraPtr->initialize();
	lightPtr->initialize();
	myLoadIConPtr->initialize();


	/*
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();
	*/
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void SceneLoad::finalize()
{
	/*
	// �^�C�g�����S�㏈��
	pLoadIcon->UninitObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->UninitCamera();
	SAFE_DELETE(pCamera);

	pLight->UninitLight();
	SAFE_DELETE(pLight);

	pTitleObj->UninitObject();
	SAFE_DELETE(pTitleObj);
	*/
}

//������������������������������������
// �X�V
//������������������������������������
void SceneLoad::update()
{
	// ���[�h�I�u�W�F�N�g�X�V
	myLoadIConPtr->update();
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
	std::thread Thred(InitNextResorce);

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
			if (isFinishedLoad)
			{
				break;
			}
		}
	}
	Thred.join();

	isFinishedLoad = false;
#endif
}

//��������������������������������������
// �`��
//��������������������������������������
void SceneLoad::draw()
{
	// �I�u�W�F�N�g�擾
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		myLoadIConPtr->draw();	// ���[�h

		cameraPtr->setCamera();		// �J�����Z�b�g

		// Direct3D�ɂ��`��̏I��
		pDevice->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//��������������������������������������������������
// ���[�h�t���O�Z�b�g
//��������������������������������������������������
void SceneLoad::setLoadFlg(bool bSet)
{
	isFinishedLoad = bSet;
}