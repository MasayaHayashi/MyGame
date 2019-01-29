//����������������������������������������������������������������
// C_StageEditor.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Editor/StageEditor.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../MainField/MainField.h"
#include "../../MyDelete/MyDelete.h"
#include "../../EditUI/WhatStageSaveUI.h"
#include "../../Goal/Goal.h"
#include <fstream>

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

//#define TEXTURE_NAME "data/TEXTURE/penguin1.png"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
StageEditor::StageEditor()
{
	// �t���O������
	nErrorFopen			= false;
	isSelectSaveStage	= false;
	uSaveStage			= static_cast<INT>(STAGE_SELECT_TYPE::STAGE_SELECT1);
	uSelectMode			= static_cast<INT>(EDIT_MODE_TYPE::MODE_EDIT);
	
	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		creteGameObj(gameObjTypeIndex);
	}

	for (UINT boardObjTypeIndex = 0; boardObjTypeIndex < GameObjectBase::MaxGameObjType; boardObjTypeIndex++)
	{
		createBoard(boardObjTypeIndex);
	}

	/*
	for (INT gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType++)
	{
		gameObjPtr[gameObjType].push_back(NEW Pawn());
	}
	*/

	/*
	// �X�e�[�W�ǂݍ��ݏ�����
	for (INT StageCnt = 0; StageCnt < MAX_STAGE; StageCnt++)
	{
		switch (StageCnt)
		{
		case 0 :
			strcpy_s(szStageFileName[StageCnt][0], STAGE_TUTORIAL_FILE_NAME);
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
	*/

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
	playerPtr->initialize();
	cameraPtr->initialize();

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->initialize();
		}
	}

	for (UINT boardTypeIndex = 0; boardTypeIndex < MaxBoardObj; boardTypeIndex++)
	{
		for (auto itr = boardObjPtr[boardTypeIndex].begin(); itr != boardObjPtr[boardTypeIndex].end(); itr++)
		{
			(*itr)->initialize();
		}
	}

	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	(*currentGameObject)->setUsedFlg(true);


//	loadStageData(1);

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


	//// �Փ˗p�N���X����
	//pCollision = NEW C_COLLISION;

	//// �|�[�Y�p�I�u�W�F�N�g������
	//pPause = NEW C_PAUSE;
	//pPause->InitObject();
	//bPause = false;

	//// ���݂̑I���u���b�N������
	//CurrentSelectType = NORMAL_BLOCK_OBJ;
	//for (INT i = 0; i < MAX_GameObjectType; i++)
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



	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->finalize();
			Mydelete::safeDelete(*itr);
		}
	}

	//// UI�֘A�㏈��
	//for (INT i = 0; i < MAX_EDITOR_UI_TYPE; i++)
	//{
	//	pUIObj[i]->UninitObject();
	//	SAFE_DELETE(pUIObj[i]);
	//}


	//// �u���b�N�㏈��
	//for (INT i = 0; i < MAX_GameObjectType; i++)
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

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->update();
		}
	}

	for (UINT boardTypeIndex = 0; boardTypeIndex < MaxBoardObj; boardTypeIndex++)
	{
		for (auto itr = boardObjPtr[boardTypeIndex].begin(); itr != boardObjPtr[boardTypeIndex].end(); itr++)
		{
			(*itr)->update();
		}
	}

	move();

	place();

	if (Keyboard::getTrigger(DIK_G))
	{
		gameObjPtr[selectGameObjType][selectGameObjIndex]->setUsedFlg(false);
		selectGameObjType ++;

		selectGameObjType = selectGameObjType % GameObjectBase::MaxGameObjType;

		updateSelectIndex();

		gameObjPtr[selectGameObjType][selectGameObjIndex]->setUsedFlg(true);
	}

	DirectX3D::printDebug("�Q�[���I�u�W�F�N�g�̎��%d", selectGameObjType);
	DirectX3D::printDebug("�Q�[���I�u�W�F�N�g�̃C���f�b�N�X%d", selectGameObjIndex);

	saveStageData(1);

	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}
	
	cameraPtr->updateStageEdit(gameObjPtr[selectGameObjType][selectGameObjIndex]->getKeyName(),selectGameObjIndex);

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
//	for (int j = 0; j < MAX_GameObjectType; j++)
//		for (int i = 0; i < MAX_GAME_OBJ; i++)
//			pGameObj[j][i]->UpdateObject();
//
//	if (uSelectMode == MODE_EDIT)
//	{
//		// ���菈��
//		D3DXVECTOR3 Cross, Normal;
//		D3DXVECTOR3 fLength;
//
//		for (INT i = 0; i < MAX_GameObjectType; i++)
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

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->draw();
		}
	}

	for (UINT boardTypeIndex = 0; boardTypeIndex < MaxBoardObj; boardTypeIndex++)
	{
		for (auto itr = boardObjPtr[boardTypeIndex].begin(); itr != boardObjPtr[boardTypeIndex].end(); itr++)
		{
			(*itr)->draw();
		}
	}

	cameraPtr->setCamera();


	//// �|�[�Y����
	//if (bPause)
	//{
	//	// �|�[�Y�`��
	//	pPause->DrawObject();
	//}

	//// �u���b�N�`��
	//for (int i = 0; i < MAX_GameObjectType; i++)
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
void StageEditor::saveStageData(size_t stageNumber)
{
	if (!Keyboard::getTrigger(DIK_O))
	{
		return;
	}

	std::ofstream exportFile;

	std::string stageString = std::to_string(stageNumber);
	exportFile.open(StagePassName + stageString + ".bin", std::ios::binary);

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			exportWorkData = (*itr)->getExportData();
			exportFile.write((CHAR*)( &exportWorkData), sizeof(ExportData));
		}
	}

	boardObjPtr[0].back()->setUsedFlg(true);

}

//����������������������������������������
// �f�[�^�ǂݍ���
//����������������������������������������
void StageEditor::loadStageData(size_t stageNumber)
{
	std::ifstream loadtFile;

	std::string stageString = std::to_string(stageNumber);
	loadtFile.open(StagePassName + stageString + ".bin", std::ios::binary);

	if (!loadtFile)
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("�X�e�[�W�f�[�^�ǂݍ��݃G���["), MB_ICONERROR);
		return;
	}

	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			loadtFile.read((CHAR*) (&exportWorkData), sizeof(ExportData));

			if (exportWorkData.isUsed)
			{
				selectGameObjIndex ++;
			}

			(*itr)->reflectionExportData(exportWorkData);
		}
	}
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
void StageEditor::move()
{
	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	D3DXVECTOR3 blockPos = (*currentGameObject)->getPosition();
	D3DXVECTOR3 blockRot = (*currentGameObject)->getRotation();
	D3DXVECTOR3 blockSize = (*currentGameObject)->getCollisionBox();

	D3DXVECTOR3 pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z);

	if (Keyboard::getPress(DIK_LSHIFT))
	{
		if (Keyboard::getPress(DIK_W))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(0.0f, 0.5f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}
		if (Keyboard::getPress(DIK_S))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(0.0f, -0.5f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}

	}

	if (Keyboard::getTrigger(DIK_W))
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z + blockSize.z);
		(*currentGameObject)->setPosition(pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getPress(DIK_A))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos - D3DXVECTOR3(0.2f, 0.0f, 0.0f));
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getTrigger(DIK_S))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z - blockSize.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getPress(DIK_D))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos + D3DXVECTOR3(0.2f, 0.0f, 0.0f));
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}


	/*
	if (GetKeyboardTrigger(DIKA))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x, BlockPos.y, BlockPos.z);
		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	}
	*/
	//// �L�[�{�[�h����
	//D3DXVECTOR3 BlockPos = gameObjPtr[selectGameObjType] [selectGameObjIndex]->GetPosition();
	//D3DXVECTOR3 BlockRot = pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->GetRotation();
	//D3DXVECTOR3 BlockSize = pGameObj[0][0]->GetCollisionBox() * 0.5f;

	//// ���ړ�
	//if (GetKeyboardTrigger(DIK_A))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x, BlockPos.y, BlockPos.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//}
	//if (GetKeyboardTrigger(DIK_D))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x + BlockSize.x, BlockPos.y, BlockPos.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//}
	//if (GetKeyboardTrigger(DIK_S))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z - BlockSize.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//}
	//if (GetKeyboardTrigger(DIK_W))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z + BlockSize.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//}

	//// �����ړ�
	//if (GetKeyboardPress(DIK_J))
	//{

	//	if (GetKeyboardPress(DIK_A))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x * 0.25f, BlockPos.y, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//	}
	//	if (GetKeyboardPress(DIK_D))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x + BlockSize.x * 0.25f, BlockPos.y, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//	}
	//	if (GetKeyboardPress(DIK_S))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z - BlockSize.z * 0.25f);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//	}
	//	if (GetKeyboardPress(DIK_W))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z + BlockSize.z * 0.25f);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//	}


	//}
	//// �㉺�ړ��A��]

	//// �ݒu
	//if (GetKeyboardTrigger(DIK_SPACE))
	//{
	//	// ��O����
	//	if (uSelectObjNum[CurrentSelectType] < MaxGameObj - 1)
	//	{
	//		uSelectObjNum[CurrentSelectType] ++;
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType] - 1]->GetPosition());
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//	}
	//}

	//// �㉺�ړ�
	//if (GetKeyboardPress(DIK_RCONTROL))
	//{
	//	if (GetKeyboardTrigger(DIK_UP))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y + BlockSize.y, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	}

	//	if (GetKeyboardTrigger(DIK_DOWN))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y - BlockSize.y * 2.0f, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	}

	//}

	//if (GetKeyboardPress(DIK_U))
	//{
	//	// ��]
	//	if (GetKeyboardTrigger(DIK_A))
	//	{
	//		D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x, BlockRot.y + D3DXToRadian(90), BlockRot.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
	//	}
	//	if (GetKeyboardTrigger(DIK_D))
	//	{
	//		D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x, BlockRot.y - D3DXToRadian(90), BlockRot.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
	//	}
	//}
}

//����������������������������������������
// �ݒu
//����������������������������������������
void StageEditor::place()
{
	if (!Keyboard::getTrigger(DIK_SPACE))
	{
		return;
	}
	
	const auto currentGameObjectType = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);
	const auto nextGameObjectType	 = std::next(gameObjPtr[selectGameObjType].begin(),	selectGameObjIndex + 1);

	D3DXVECTOR3 blockPos  = (*currentGameObjectType)->getPosition();
	D3DXVECTOR3 blockRot  = (*currentGameObjectType)->getRotation();
	D3DXVECTOR3 blockSize = (*currentGameObjectType)->getCollisionBox();

	(*currentGameObjectType)->setPosition((*currentGameObjectType)->getPosition());
	(*currentGameObjectType)->setUsedFlg(true);
	selectGameObjIndex++;

	(*nextGameObjectType)->setPosition((*currentGameObjectType)->getPosition());
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
	//	CurrentSelectType = CurrentSelectType % MAX_GameObjectType;
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
	case static_cast<INT>( GameObjectType::NormalBlockObj ) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[0].push_back(NEW MainObject("box.x","IceStone.png",objIndex, GameObjectType::NormalBlockObj,false));
		}
		break;
	case static_cast<INT>(GameObjectType::MoveBlockOBj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[1].push_back(NEW MainObject("flatAndHill.x", "double_1.png",objIndex, GameObjectType::MoveBlockOBj,true));
		}
		break;
	case static_cast<INT>((GameObjectType::FieldObj)):
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[2].push_back(NEW MainObject("flat.x","double_1.png",objIndex, GameObjectType::FieldObj,true));
		}
		break;
	case static_cast<INT>((GameObjectType::GoalObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[3].push_back(NEW Goal("heart.x", "heart.png", objIndex));
		}
		break;
	default:
		break;
	}
}

//����������������������������������������������������
// �{�[�h����
//����������������������������������������������������
void StageEditor::createBoard(size_t objType)
{
	switch (objType)
	{
	case static_cast<INT>( BoardObjType::WhatSave):
	{
		boardObjPtr[objType].push_back(std::unique_ptr<WhatStageSaveUi>(NEW WhatStageSaveUi()));
	}
	break;

	default:
		break;
	}
}

//��������������������������������������������
// 
//��������������������������������������������
void StageEditor::move(INT input)
{
	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	D3DXVECTOR3 blockPos = (*currentGameObject)->getPosition();
	D3DXVECTOR3 blockRot = (*currentGameObject)->getRotation();
	D3DXVECTOR3 blockSize = (*currentGameObject)->getCollisionBox();

	switch (input)
	{
	case DIK_W:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z + blockSize.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	case DIK_A:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x - blockSize.x, blockPos.y, blockPos.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	case DIK_S:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z - blockSize.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	case DIK_D:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x + blockSize.x, blockPos.y, blockPos.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	default:
		break;
	}
}

//��������������������������������������������
// �g�p�I�u�W�F�N�g�X�V
//��������������������������������������������
void StageEditor::updateSelectIndex()
{
	selectGameObjIndex = 0;

	for (auto gameObj : gameObjPtr[selectGameObjType])
	{
		if (gameObj->getUsedFlg())
		{
			selectGameObjIndex ++;
		}
		else
		{
			return;
		}
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