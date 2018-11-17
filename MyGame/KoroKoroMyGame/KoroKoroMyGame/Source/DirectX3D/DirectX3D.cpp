//����������������������������������������
// DirectX3D.cpp
// DirectX9.0c�p�N���X
// Author : Masaya Hayashi
//����������������������������������������

// ===== �C���N���[�h�� =====
#include "DirectX3D.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"

// ===== �ÓI�����o�ϐ� =====
Microsoft::WRL::ComPtr<IDirect3DDevice9>   DirectX3D::directXDevice  = nullptr;
Microsoft::WRL::ComPtr<ID3DXEffect>		   DirectX3D::directXEffect  = nullptr;

CHAR DirectX3D::debug[1024] = {" \0 "};

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
HRESULT DirectX3D::initialize(HWND& wnd)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	directXObj = Direct3DCreate9(D3D_SDK_VERSION);
	
	if (directXObj == nullptr)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	directXObj->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

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
	if (FAILED( directXObj.Get()->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		wnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, &directXDevice)))
	{
		// ��L�̐ݒ肪���s������`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED( directXObj->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			wnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &directXDevice )))
		{
			// ��L�̐ݒ肪���s������`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED( directXObj->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				wnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &directXDevice )))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	//�V�F�[�_�[��ǂݍ���
	if (FAILED(D3DXCreateEffectFromFile( directXDevice.Get(), "Test.fx", nullptr, nullptr, 0, nullptr, &directXEffect, nullptr)))
	{
		MessageBox(nullptr, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);
		return E_FAIL;
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�	
	directXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// ���ʂ��J�����O
	directXDevice->SetRenderState(D3DRS_ZENABLE, TRUE);					// Z�o�b�t�@���g�p
	directXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	directXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	directXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	directXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);			// �@������ɐ��K������ݒ�
	
	directXDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	directXDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	directXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	directXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�
	
	directXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	directXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	directXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	initializeDebugProc();
	/*
	// �X�e�[�W�ԍ�������
	g_StageNum = 0;	// �`���[�g���A���X�e�[�W

	// ���͂̏���������
	initializeInput(hInstance, hWnd);

	// Xinput����
	Xinput::Create();

	// Xinput������
	g_pXinput = Xinput::GetInstance();
	g_pXinput->initializeXinput();

#if _DEBUG
		// �f�o�b�O�\���̏���������
		initializeDebugProc();
#endif

	// �T�E���h�����̏�����
	g_Audio = NEW C_AUDIO();

	// �V�[��������
	g_pSceneManager = NEW C_SCENE_MANAGER();
	g_pSceneManager->initialize();

	// ���[�h�V�[��������
	g_pSceneLoad = NEW C_SCENE_LOAD;
	g_pSceneLoad->initialize();

	return S_OK;
	*/

	return E_FAIL;
}

//����������������������������
// �X�V 
//����������������������������
void DirectX3D::update()
{
	SceneManager::update();
}

//����������������������������
// �`��
//����������������������������
void DirectX3D::draw() const
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	directXDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(directXDevice->BeginScene()))
	{
		// �V�[���`��
		SceneManager::draw();

		// �t�F�[�h�`��
	//	g_pSceneManager->DrawFadeScene();

		// �f�o�b�O�\���̕`�揈��
#if _DEBUG
		//if (g_bDispDebug)
		drawDebugProc();
#endif

		// Direct3D�ɂ��`��̏I��
		directXDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	directXDevice->Present(nullptr, nullptr, nullptr, nullptr);
	
}

//����������������������������������
// �f�o�b�O������
//����������������������������������
HRESULT DirectX3D::initializeDebugProc()
{
	HRESULT hr;

	// ���\���p�t�H���g��ݒ�
	hr = D3DXCreateFont(directXDevice.Get(), 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
						OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &directXFont);

	// ���N���A
	memset(debug, 0, sizeof debug);
	

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
void DirectX3D::drawDebugProc() const
{
	RECT rect = { 0, 0, Application::ScreenWidth, Application::ScreenHeight };

	// ���\��
	directXFont->DrawText(nullptr, debug, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0x00));

	// ���N���A
	memset(debug, 0, sizeof debug);
}

//����������������������������������
// �f�o�b�O�㏈��
//����������������������������������
void DirectX3D::finalizeDebugProc()
{

}

//����������������������������������
// �f�o�b�O�`��
//����������������������������������
void DirectX3D::printDebug(CHAR *fmt,...)
{
	va_list list;			// �ψ�������������ׂɎg�p����ϐ�
	char *pCur;
	char aBuf[256] = { "\0" };
	char aWk[32];

	// �ψ����ɃA�N�Z�X����O�̏�������
	va_start(list, fmt);

	pCur = fmt;
	for (; *pCur; ++pCur)
	{
		if (*pCur != '%')
		{
			sprintf_s(aWk, "%c", *pCur);
		}
		else
		{
			pCur++;

			switch (*pCur)
			{
			case 'd':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%d", va_arg(list, int));
				break;

			case 'f':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%.2f", va_arg(list, double));		// double�^�Ŏw��
				break;

			case 's':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%c", va_arg(list, char));
				break;

			default:
				sprintf_s(aWk, "%c", *pCur);
				break;
			}
		}
		strcat_s(aBuf, aWk);
	}

	// �ψ����ɃA�N�Z�X������̏I������
	va_end(list);

	// �A��
	if ((strlen(debug) + strlen(aBuf)) < ((sizeof debug) - 1))
	{
		strcat_s(debug, aBuf);
	}
}

//����������������������������������
// �f�o�C�X�擾
//����������������������������������
LPDIRECT3DDEVICE9 DirectX3D::getDevice()
{
	return directXDevice.Get();
}

//����������������������������������
// �V�F�[�_�[�peffect�擾
//����������������������������������
LPD3DXEFFECT DirectX3D::getEffect()
{
	return directXEffect.Get();
}