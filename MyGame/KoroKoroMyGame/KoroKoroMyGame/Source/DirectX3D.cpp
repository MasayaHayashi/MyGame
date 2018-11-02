//����������������������������������������
// DirectX3D.cpp
// DirectX9.0c�p�N���X
// Author : Masaya Hayashi
//����������������������������������������

// ===== �C���N���[�h�� =====
#include "DirectX3D.h"
#include "Application.h"

// ===== �ÓI�����o�ϐ� =====
//std::unique_ptr<LPDIRECT3DDEVICE9> DirectX3D::pDirectXDevice(new LPDIRECT3DDEVICE9);


//����������������������������������
// �R���X�g���N�^
//����������������������������������
DirectX3D::DirectX3D()
{
	pDirectXDevice.reset(new LPDIRECT3DDEVICE9);
	pDirectXObj.reset(new LPDIRECT3D9);
	pEffectObj.reset(new LPD3DXEFFECT);
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
	*pDirectXObj.get() = Direct3DCreate9(D3D_SDK_VERSION);


	if (*pDirectXObj == nullptr)
		return E_FAIL;

	// ���݂̃f�B�X�v���C���[�h���擾
	(*pDirectXObj)->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);


	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount  = 1;								// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth  = (Application::ScreenWidth);	// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = (Application::ScreenHeight);	// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;					// �J���[���[�h�̎w��
	d3dpp.SwapEffect	   = D3DSWAPEFFECT_DISCARD;			// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed		   = isWindowMode;
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
	if (FAILED( (*pDirectXObj)->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		wnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, pDirectXDevice.get())))
	{
		// ��L�̐ݒ肪���s������`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED( (*pDirectXObj)->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			wnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, pDirectXDevice.get() )))
		{
			// ��L�̐ݒ肪���s������`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED( (*pDirectXObj)->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				wnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, pDirectXDevice.get() )))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}


	//�V�F�[�_�[��ǂݍ���
	if (FAILED(D3DXCreateEffectFromFile( *pDirectXDevice.get(), "Data/FX/Min.fx", nullptr, nullptr, 0, nullptr, pEffectObj.get(), nullptr)))
	{
		MessageBox(nullptr, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�	
	(*pDirectXDevice.get())->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// ���ʂ��J�����O
	(*pDirectXDevice.get())->SetRenderState(D3DRS_ZENABLE, TRUE);					// Z�o�b�t�@���g�p
	(*pDirectXDevice.get())->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	(*pDirectXDevice.get())->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	(*pDirectXDevice.get())->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	(*pDirectXDevice.get())->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);			// �@������ɐ��K������ݒ�

	(*pDirectXDevice.get())->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	(*pDirectXDevice.get())->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	(*pDirectXDevice.get())->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	(*pDirectXDevice.get())->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�
	
	
	(*pDirectXDevice.get())->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	(*pDirectXDevice.get())->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	(*pDirectXDevice.get())->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	
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

	return E_FAIL;
}

//����������������������������
// �`��
//����������������������������
const void DirectX3D::draw()
{
	
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	(*pDirectXDevice.get())->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED((*pDirectXDevice.get())->BeginScene()))
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
		(*pDirectXDevice.get())->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	(*pDirectXDevice.get())->Present(nullptr, nullptr, nullptr, nullptr);
	
}

//����������������������������������
// �f�o�b�O������
//����������������������������������
HRESULT DirectX3D::initDebugProc()
{
	HRESULT hr;

	/*

	// ���\���p�t�H���g��ݒ�
	hr = D3DXCreateFont(pDirectXDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		 OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &pD3DXFont);

	// ���N���A
	memset(debug, 0, sizeof debug);

	return E_FAIL;
	*/

	return E_FAIL;
}

//����������������������������������
// �f�o�b�O�X�V
//����������������������������������
void DirectX3D::updateDebugProc()
{

}

//����������������������������������
// �f�o�b�O�`��
//����������������������������������
const void DirectX3D::drawDebugProc()
{

}

//����������������������������������
// �f�o�b�O�㏈��
//����������������������������������
void DirectX3D::finalizeDebugProc()
{

}

//����������������������������������
// �f�o�C�X�擾
//����������������������������������
LPDIRECT3DDEVICE9 DirectX3D::getDevice()
{
	//return pDirectXDevice;

	return nullptr;
}