//����������������������������������������������������������������
// C_StageEditor.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

#define _CRT_SECURE_NO_WARNINGS

// ===== �C���N���[�h�� =====
#include "../Editor/StageEditor.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../Block/Block.h"

//
//#include "C_MoveBlock.h"
//#include "C_ItemStar.h"
//#include "C_GoalObj.h"
//#include "C_Pause.h"
//#include "C_Enemy.h"
//#include "C_WhatStageLoadUI.h"
//#include "C_WhatStageSaveUI.h"
//#include "C_StageUI.h"
//#include "C_NumbersUIBase.h"
//#include "C_SaveOkUI.h"
//#include "C_StageEditUI.h"
//#include "C_EditDescrUI.h"
//#include "input.h"
//#include "collision.h"

// ===== �萔�E�}�N����` =====
#define MODEL_NAME				 "data/MODEL/Qube.x"
#define PRESS_MOVE_SPEED (0.5f)

//#define TEXTURE_NAME "data/TEXTURE/penguin1.png"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
StageEditor::StageEditor()
{
	// �t���O������
	nErrorFopen			= false;
	bIsSelectSaveStage	= false;
	uSaveStage  = STAGE_SELECT1;
	uSelectMode = MODE_EDIT;
	
	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		creteGameObj(gameObjTypeIndex);
	}

	/*
	for (INT gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType++)
	{
		gameObjPtr[gameObjType].push_back(NEW Pawn());
	}
	*/

	// �X�e�[�W�ǂݍ��ݏ�����
	for (INT StageCnt = 0; StageCnt < MAX_STAGE; StageCnt++)
	{
		switch (StageCnt)
		{
		case 0 :
			strcpy(&szStageFileName[StageCnt][0], STAGE_TUTORIAL_FILE_NAME);
			break;
		case 1:
			strcpy(&szStageFileName[StageCnt][0], STAGE_1_FILE_NAME);
			break;
		case 2:
			strcpy(&szStageFileName[StageCnt][0], STAGE_2_FILE_NAME);
			break;
		case 3:
			strcpy(&szStageFileName[StageCnt][0], STAGE_3_FILE_NAME);
			break;

		default:
			break;
		}
	}

	lightPtr.reset(NEW Light());
	cameraPtr.reset(NEW Camera());
	playerPtr.reset(NEW Player(D3DXVECTOR3(0.0f,5.0f,0.0f),0));
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
StageEditor::~StageEditor()
{

}

//����������������������������������������������
// �v���C���[������
//����������������������������������������������
void StageEditor::initialize()
{
	lightPtr->initialize();
	cameraPtr->initialize();
	playerPtr->initialize();

	/*
	// UI������
	for (INT UiCnt = 0; UiCnt < MAX_EDITOR_UI_TYPE; UiCnt++)
	{
		switch (UiCnt)
		{
			case UI_WTHAT_STAGE_SAVE:
				pUIObj[UiCnt] = NEW C_WHAT_STAGE_SAVE_UI;
				break;
			case UI_WTHAT_STAGE_LOAD:
				pUIObj[UiCnt] = NEW C_WHAT_STAGE_LOAD_UI;
				break;
			case UI_STAGE:
				pUIObj[UiCnt] = NEW C_STAGE_UI;
				break;
			case UI_NUMBERS:
				pUIObj[UiCnt] = NEW C_NUMBERS_UI_BASE;
				break;
			case UI_SAVE_OK:
				pUIObj[UiCnt] = NEW C_SAVE_OK_UI;
				break;
			case UI_STAGE_EDIT:
				pUIObj[UiCnt] = NEW C_STAGE_EDIT_UI;
				break;
			case UI_DESCR:
				pUIObj[UiCnt] = NEW C_EDIT_DESCR_UI;
				break;
			default:
				break;
		}
		pUIObj[UiCnt]->InitObject();
	}
	*/

	/*
	// �u���b�N������
	for (auto mapItr = gameObjPtr.begin(); mapItr != gameObjPtr.end(); mapItr ++)
	{
		for (auto listItr = mapItr->second.begin(); listItr != mapItr->second.end(); listItr++)
		{
			listItr->initialize();
		}
	}
	*/

	//// �Փ˗p�N���X����
	//pCollision = NEW C_COLLISION;

	//// �|�[�Y�p�I�u�W�F�N�g������
	//pPause = NEW C_PAUSE;
	//pPause->InitObject();
	//bPause = false;

	//// ���݂̑I���u���b�N������
	//CurrentSelectType = NORMAL_BLOCK_OBJ;
	//for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
	//	uSelectObjNum[i] = 0;

	//// �g�p�t���O�Z�b�g
	//pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//// �X�e�[�W�ǂݍ���
	//LoadStageData(0);

	// BGM�Đ�
//	PlaySound(SOUND_BGM_EDIT);
}

//��������������������������������������������
// �v���C���[�㏈��
//��������������������������������������������
void StageEditor::finalize()
 {
	cameraPtr->finalize();

	lightPtr->finalize();

	playerPtr->finalize();


	//// UI�֘A�㏈��
	//for (INT i = 0; i < MAX_EDITOR_UI_TYPE; i++)
	//{
	//	pUIObj[i]->UninitObject();
	//	SAFE_DELETE(pUIObj[i]);
	//}


	//// �u���b�N�㏈��
	//for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
	//{
	//	for (INT j = 0; j < MAX_GAME_OBJ; j++)
	//	{
	//		pGameObj[i][j]->UninitObject();
	//		SAFE_DELETE(pGameObj[i][j]);
	//	}
	//}

	//// �|�[�Y�㏈��
	//pPause->UninitObject();
	//SAFE_DELETE(pPause);

}

//����������������������������������������������
// �v���C���[�X�V
//����������������������������������������������
void StageEditor::update()
{
	playerPtr->update(cameraPtr->getFowerd());

//	cameraPtr->update();
//{
//	// �|�[�Y��������
//	if (GetKeyboardTrigger(DIK_P))
//		bPause = !bPause;
//
//	// �|�[�Y�X�V
//	if (bPause)
//	{
//		pPause->UpdateObject();
//		return;
//	}
//
//	// �v���C���[�X�V
//	pPlayer->UpdatePlayer_SceneEdit();
//
//	// �J�����X�V
//	pCamera->UpdateCamera_StageEdit(pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->GetPosition());
//
//	// UI�֘A�X�V
//	for (INT i = 0; i < MAX_EDITOR_UI_TYPE; i++)
//		pUIObj[i]->UpdateObject();
//
//	// �u���b�N�X�V
//	for (int j = 0; j < MAX_GAME_OBJ_TYPE; j++)
//		for (int i = 0; i < MAX_GAME_OBJ; i++)
//			pGameObj[j][i]->UpdateObject();
//
//	if (uSelectMode == MODE_EDIT)
//	{
//		// ���菈��
//		D3DXVECTOR3 Cross, Normal;
//		D3DXVECTOR3 fLength;
//
//		for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
//			for (INT j = 0; j < MAX_GAME_OBJ; j++)
//			{
//				// ��O����
//				if (!pGameObj[i][j]->GetUsedFlg())
//					continue;
//
//				// �v���C���[�ƃu���b�N�Ƃ̔���
//				pCollision->CheckCollisionField(pPlayer, NULL, pGameObj[i][j], Cross, Normal, fLength,D3DXVECTOR3(0.0f,-1.0f,0.0f));
//			}
//
//		// �I���I�u�W�F�N�g�؂�ւ�
//		ChangeSelectObj();
//
//		// �ړ�
//		MoveObj();
//
//		// �폜
//		DeleteObj();
//	}
//
//	if (GetKeyboardTrigger(DIK_O))
//		// ���[�h�ؑ�
//		uSelectMode = MODE_SAVE_CONF;
//
//	// �������݃X�e�[�W�m�F
//	if (uSelectMode == MODE_SAVE_CONF)
//	{
//		// �Z�[�u���邩�̊m�F
//		CheckSaveData();
//
//		// ��������
//		if (bIsSelectSaveStage)
//			SaveStageData();
//	}
}

//����������������������������������
// �X�e�[�W�֘A�`��
//����������������������������������
void StageEditor::draw()
{
	playerPtr->draw();

	cameraPtr->setCamera();
	//// �|�[�Y����
	//if (bPause)
	//{
	//	// �|�[�Y�`��
	//	pPause->DrawObject();
	//}

	//// �u���b�N�`��
	//for (int i = 0; i < MAX_GAME_OBJ_TYPE; i++)
	//	for (int j = 0; j < MAX_GAME_OBJ; j++)
	//		pGameObj[i][j]->DrawObject();

	//// UI�֘A�`��
	//for (INT i = 0; i < MAX_EDITOR_UI_TYPE; i++)
	//	pUIObj[i]->DrawObject();

	//// �J�����Z�b�g
	//pCamera->SetCamera();

}


//����������������������������������������
// �X�e�[�W�f�[�^�����o��
//����������������������������������������
void StageEditor::saveStageData()
{
//{
//	// �t�@�C���I�[�v��
//	nErrorFopen = fopen_s(&fp,&szStageFileName[uSaveStage + 1][0], "wb");
//
//	// ��O����
//	if (nErrorFopen)
//		return;
//
//	for (int j = 0; j < MAX_GAME_OBJ_TYPE; j++)
//		for (int i = 0; i < MAX_GAME_OBJ; i++)
//			fwrite(pGameObj[j][i]->GetExportData(), sizeof(EXPORT_GAMEOBJ_DATA), 1, fp);
//
//	// �t�@�C���N���[�Y
//	fclose(fp);
}

//����������������������������������������
// �X�e�[�W�f�[�^�ǂݍ���
//����������������������������������������
void StageEditor::loadStageData(UINT LoadStageNum)
{
//	// �t�@�C���I�[�v��
//	nErrorFopen = fopen_s(&fp, &szStageFileName[LoadStageNum + 1][0], "rb");
//
//	// ��O����
//	if (nErrorFopen)
//		return;
//
//#if 1
//
//	// �t�@�C���ǂݍ���
//	INT nLoadCnt[MAX_GAME_OBJ_TYPE] = { 0, 0, 0, };
//
//	for (INT i = 0; i < MAX_GAME_OBJ * MAX_GAME_OBJ_TYPE; i++)
//	{
//		fread(&ExportWorkData, sizeof(EXPORT_GAMEOBJ_DATA), 1, fp);
//		switch (ExportWorkData.ObjType)
//		{
//		case NORMAL_BLOCK_OBJ:
//			// �ǂݍ��݃f�[�^�Z�b�g
//			pGameObj[NORMAL_BLOCK_OBJ][nLoadCnt[NORMAL_BLOCK_OBJ]]->SetExportData(ExportWorkData);
//
//			// �I��ԍ��X�V
//			if (pGameObj[NORMAL_BLOCK_OBJ][nLoadCnt[NORMAL_BLOCK_OBJ]]->GetUsedFlg())
//				uSelectObjNum[NORMAL_BLOCK_OBJ] ++;
//
//			// �ǂݍ��݃J�E���^�X�V
//			nLoadCnt[NORMAL_BLOCK_OBJ] ++;
//
//			break;
//
//		case MOVE_BLOCK_OBJ:
//			// �ǂݍ��݃f�[�^�Z�b�g
//			pGameObj[MOVE_BLOCK_OBJ][nLoadCnt[MOVE_BLOCK_OBJ]]->SetExportData(ExportWorkData);
//
//			// �I��ԍ��X�V
//			if (pGameObj[MOVE_BLOCK_OBJ][nLoadCnt[MOVE_BLOCK_OBJ]]->GetUsedFlg())
//				uSelectObjNum[MOVE_BLOCK_OBJ] ++;
//
//			// �ǂݍ��݃J�E���^�X�V
//			nLoadCnt[MOVE_BLOCK_OBJ] ++;
//
//			break;
//
//		case STAR_OBJ:
//			// �ǂݍ��݃f�[�^�Z�b�g
//			pGameObj[STAR_OBJ][nLoadCnt[STAR_OBJ]]->SetExportData(ExportWorkData);
//
//			// �I��ԍ��X�V
//			if (pGameObj[STAR_OBJ][nLoadCnt[STAR_OBJ]]->GetUsedFlg())
//				uSelectObjNum[STAR_OBJ] ++;
//
//			// �ǂݍ��݃J�E���^�X�V
//			nLoadCnt[STAR_OBJ] ++;
//
//			break;
//		default:
//			break;
//		}
//	}
//
//#endif
//
//	// �t�@�C���N���[�Y
//	fclose(fp);
}

//����������������������������������������
// �����o�����邩�̃`�F�b�N
//����������������������������������������
void StageEditor::checkSaveData()
{
//{
//	// UI�\��
//	pUIObj[UI_WTHAT_STAGE_SAVE]->SetUsedFlg(true);
//	pUIObj[UI_STAGE]->SetUsedFlg(true);
//	pUIObj[UI_NUMBERS]->SetUsedFlg(true);
//
//	if (GetKeyboardTrigger(DIK_D))
//		uSaveStage ++;
//	if (GetKeyboardTrigger(DIK_A))
//		uSaveStage --;
//
//	
//	uSaveStage %= MAX_STAGE - 1;	// �`���[�g���A���͑I�������Ȃ�
//
//	// �����\���ؑւ�
//	pUIObj[UI_NUMBERS]->SetCurrentAnimPattern(uSaveStage + 1);
//
//	// �Z�[�u
//	if (GetKeyboardTrigger(DIK_I))
//	{
//		bIsSelectSaveStage = true;
//		uSelectMode = MODE_EDIT;
//
//		// UI��\��
//		pUIObj[UI_WTHAT_STAGE_SAVE]->SetUsedFlg(false);
//		pUIObj[UI_STAGE]->SetUsedFlg(false);
//		pUIObj[UI_NUMBERS]->SetUsedFlg(false);
//
//		pUIObj[UI_SAVE_OK]->SetUsedFlg(true);
//	}
//	// ���̉�ʂ�
//	if (GetKeyboardTrigger(DIK_U))
//	{
//		// UI��\��
//		pUIObj[UI_WTHAT_STAGE_SAVE]->SetUsedFlg(false);
//		pUIObj[UI_STAGE]->SetUsedFlg(false);
//		pUIObj[UI_NUMBERS]->SetUsedFlg(false);
//
//		uSelectMode = MODE_EDIT;
//	}
}

//����������������������������������������
// �I�u�W�F�N�g�폜
//����������������������������������������
void StageEditor::deleteObj()
{
	/*
	if (GetKeyboardPress(DIK_J))
	{
		if (GetKeyboardTrigger(DIK_K))
		{
			for (int i = 0; i < MAX_GAME_OBJ; i++)
			{
				// ��O����
				if (!pGameObj[CurrentSelectType][i]->GetUsedFlg() || uSelectObjNum[CurrentSelectType] == i)
					continue;

				pCollision->CheckCollisionBlock(pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]], pGameObj[CurrentSelectType][i]);
			}
		}
	}
	*/

}

//��������������������������������������������������
// �I�u�W�F�N�g�ړ�
//��������������������������������������������������
void StageEditor::moveObj()
{
//{
//	// �L�[�{�[�h����
//	D3DXVECTOR3 BlockPos = pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->GetPosition();
//	D3DXVECTOR3 BlockRot = pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->GetRotation();
//	D3DXVECTOR3 BlockSize = pGameObj[0][0]->GetCollisionBox() * 0.5f;
//
//	// ���ړ�
//	if (GetKeyboardTrigger(DIK_A))
//	{
//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x, BlockPos.y, BlockPos.z);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//	}
//	if (GetKeyboardTrigger(DIK_D))
//	{
//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x + BlockSize.x, BlockPos.y, BlockPos.z);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//
//	}
//	if (GetKeyboardTrigger(DIK_S))
//	{
//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z - BlockSize.z);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//
//	}
//	if (GetKeyboardTrigger(DIK_W))
//	{
//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z + BlockSize.z);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//	}
//
//	// �����ړ�
//	if (GetKeyboardPress(DIK_J))
//	{
//
//		if (GetKeyboardPress(DIK_A))
//		{
//			D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x * 0.25f, BlockPos.y, BlockPos.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//		}
//		if (GetKeyboardPress(DIK_D))
//		{
//			D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x + BlockSize.x * 0.25f, BlockPos.y, BlockPos.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//
//		}
//		if (GetKeyboardPress(DIK_S))
//		{
//			D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z - BlockSize.z * 0.25f);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//
//		}
//		if (GetKeyboardPress(DIK_W))
//		{
//			D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z + BlockSize.z * 0.25f);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//		}
//
//
//	}
//	// �㉺�ړ��A��]
//
//	// �ݒu
//	if (GetKeyboardTrigger(DIK_SPACE))
//	{
//		// ��O����
//		if (uSelectObjNum[CurrentSelectType] < MAX_GAME_OBJ - 1)
//		{
//			uSelectObjNum[CurrentSelectType] ++;
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType] - 1]->GetPosition());
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
//		}
//	}
//
//	// �㉺�ړ�
//	if (GetKeyboardPress(DIK_RCONTROL))
//	{
//		if (GetKeyboardTrigger(DIK_UP))
//		{
//			D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y + BlockSize.y, BlockPos.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//		}
//
//		if (GetKeyboardTrigger(DIK_DOWN))
//		{
//			D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y - BlockSize.y * 2.0f, BlockPos.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
//		}
//
//	}
//
//	if (GetKeyboardPress(DIK_U))
//	{
//		// ��]
//		if (GetKeyboardTrigger(DIK_A))
//		{
//			D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x, BlockRot.y + D3DXToRadian(10), BlockRot.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
//		}
//		if (GetKeyboardTrigger(DIK_D))
//		{
//			D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x, BlockRot.y - D3DXToRadian(10), BlockRot.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
//		}
//
//		if (GetKeyboardTrigger(DIK_W))
//		{
//			D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x + D3DXToRadian(10), BlockRot.y, BlockRot.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
//		}
//		if (GetKeyboardTrigger(DIK_S))
//		{
//			D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x + D3DXToRadian(10), BlockRot.y , BlockRot.z);
//			pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
//		}
//	}
}

//����������������������������������������
// ��]
//����������������������������������������
void StageEditor::rotObj()
{

}

//����������������������������������������
// �I���I�u�W�F�N�g�؂�ւ�
//����������������������������������������
void StageEditor::changeSelectObj()
{
	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// ���݂̃u���b�N�𖢎g�p��
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(false);

	//	CurrentSelectType++;
	//	CurrentSelectType = CurrentSelectType % MAX_GAME_OBJ_TYPE;
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//}
}

//����������������������������������������
// �J�����擾
//����������������������������������������
Camera* StageEditor::getCamera()
{
	return cameraPtr.get();
}

//����������������������������������������
// �Q�[���I�u�W�F�N�g����
//����������������������������������������
void StageEditor::creteGameObj(size_t objType)
{
	switch (objType)
	{
	case static_cast<INT>( GAME_OBJ_TYPE::NormalBlockObj ):
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[objType].push_back(NEW Block());
		}

		break;
	case static_cast<INT>(GAME_OBJ_TYPE::MoveBlockOBj):
		break;
	case static_cast<INT>((GAME_OBJ_TYPE::StarObj)):
		break;
	case static_cast<INT>((GAME_OBJ_TYPE::GoalObj)):
		break;
	default:
		break;
	}
}

/*
template <class T>
void StageEditor::creteGamesObj(T dataType,size_t size)
{
	for (INT objIndex = 0; objIndex < size; objIndex ++)
	{
		gameObjPtr[objIndex].push_back(NEW T);
	}
	break;
}
*/