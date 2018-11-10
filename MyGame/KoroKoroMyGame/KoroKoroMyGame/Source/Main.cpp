//������������������������������������������
// �G���g���[�|�C���g
// main.cpp
// Author : Masaya Hayashi
//������������������������������������������

// ===== �C���N���[�h�� =====
#include <Windows.h>
#include "Application.h"
#include "DirectX3D.h"

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	Application applivationObj;

	applivationObj.initialize(instance,cmdShow);

	while (applivationObj.isAppContinuation())
	{
		applivationObj.mainLoop();
		
	}

	applivationObj.finalize();

	return 0;
}