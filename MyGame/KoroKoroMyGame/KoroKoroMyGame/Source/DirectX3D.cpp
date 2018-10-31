//����������������������������������������
// DirectX3D.cpp
// DirectX9.0c�p�N���X
// Author : Masaya Hayashi
//����������������������������������������

// ===== �C���N���[�h�� =====
#include "DirectX3D.h"
#include "Source\Application.h"

//����������������������������������
// �R���X�g���N�^
//����������������������������������
DirectX3D::DirectX3D()
{

}

//����������������������������������
// �f�X�g���N�^
//����������������������������������
DirectX3D::~DirectX3D()
{
	
}

//����������������������������������
// ������
//����������������������������������
HRESULT DirectX3D::init(HWND& wnd)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (pD3D == nullptr)
		return E_FAIL;

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount  = 1;								// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth  = (Application::ScreenWidth);		// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = (Application::ScreenHeight);		// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;					// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;				// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = isWindowMode;
	d3dpp.EnableAutoDepthStencil = TRUE;					// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if (isWindowMode)
	{
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}


	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		wnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, &pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			wnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				wnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	//�V�F�[�_�[��ǂݍ���
	if (FAILED(D3DXCreateEffectFromFile(pD3DDevice, "Data/FX/Min.fx", nullptr, nullptr, 0, nullptr, &pEffect, nullptr)))
	{
		MessageBox(nullptr, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// ���ʂ��J�����O
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);					// Z�o�b�t�@���g�p
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);			// �@������ɐ��K������ݒ�

	
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�


	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	/*
	// �X�e�[�W�ԍ�������
	g_StageNum = 0;	// �`���[�g���A���X�e�[�W

	// ���͂̏���������
	InitInput(hInstance, hWnd);

	// Xinput����
	C_XINPUT::Create();

	// Xinput������
	g_pXinput = C_XINPUT::GetInstance();
	g_pXinput->InitXinput();

#if _DEBUG
		// �f�o�b�O�\���̏���������
		InitDebugProc();
#endif

	// �T�E���h�����̏�����
	g_Audio = NEW C_AUDIO();

	// �V�[��������
	g_pSceneManager = NEW C_SCENE_MANAGER();
	g_pSceneManager->InitScene();

	// ���[�h�V�[��������
	g_pSceneLoad = NEW C_SCENE_LOAD;
	g_pSceneLoad->InitScene();

	return S_OK;
	*/
}

//����������������������������
// �`��
//����������������������������
const void DirectX3D::draw()
{

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pD3DDevice->BeginScene()))
	{
		// �V�[���`��
	//	pSceneManager->DrawScene();

		// �t�F�[�h�`��
	//	g_pSceneManager->DrawFadeScene();

		// �f�o�b�O�\���̕`�揈��
#if _DEBUG
		//if (g_bDispDebug)
		//	DrawDebugProc();
#endif


		// Direct3D�ɂ��`��̏I��
		pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//����������������������������������
// �f�o�b�O������
//����������������������������������
HRESULT DirectX3D::initDebugProc()
{

	return E_FAIL;
}


//����������������������������������
// �f�o�b�O���O�`��
//����������������������������������
const void DirectX3D::drawDebugProc()
{

}