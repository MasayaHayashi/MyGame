//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// StarItem.cpp
// スターアイテム
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "StarItem.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include "../KeyBoard/Keyboard.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StarItem::StarItem()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
// modelPass:	 モデルパス入力
// texturePass : テクスチャパス入力
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StarItem::StarItem(std::string modelPass,std::string texturePass, size_t setNumber,GameObjectType setGameObj,bool setFieldModel)
{
	tagName						= modelPass;
	idNumber					= setNumber;
	modelPasses[tagName]		= passName + modelPass;
	texturePasses[tagName]		= passName + texturePass;
	myGameObjType				= setGameObj;
	isFieldObject				= setFieldModel;
	
	myTransform.isUsed = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StarItem::~StarItem()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarItem::initialize()
{
	// ファイルパス設定
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, texturePasses[tagName].c_str());

	// モデル生成
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	Collision::registerList(&myTransform, tagName);

	myTransform.scale	 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos		 = D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);
	myTransform.rotDeg.y = 90.0f;
	myTransform.collisionBox = meshDataObj.collitionBox;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarItem::finalize()
{
	ResourceManager::destroyAllMesh();

	modelPasses.clear();
	texturePasses.clear();
	ResourceManager::destroyAllTexture();

	Collision::release(tagName);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarItem::update()
{
	updateExportData();

	if (!myTransform.isUsed)
	{
		return;
	}

	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	if (idNumber == Collision::getHitIndex("star.x"))
	{
		myTransform.isUsed = false;
		GameManager::changeGameType(GameManager::GameType::GetItem);
	}

	myTransform.rotDeg.y += 0.1f;
	D3DXMATRIX matrix;
	D3DXMatrixRotationY(&matrix, D3DXToRadian(myTransform.rotDeg.y));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &matrix);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarItem::draw()
{
	if (!myTransform.isUsed)
	{
		return;
	}

	Pawn::draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarItem::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	if (!myTransform.isUsed)
	{
		return;
	}

	// 描画
	Pawn::draw(mtxView,mtxProj);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// キー値取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const std::string StarItem::getKeyName()
{
	return tagName;
}