//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Field.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef FIELD_H
#define FIELD_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"

// ===== クラスの前方宣言 =====

// ===== 構造体定義 =====

// ===== 列挙体定義 =====

// ===== クラス定義 =====
class Field final : public Pawn
{
public:
	Field();
	~Field();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj);

private:

	const std::string ModelFilePass		= "Data/Model/Field/RockLayered_5.x.x";
	const std::string TextureFilePass	= "Data/Model/Heart/heart.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	void initializeTitle();
	void initializeGameMain();
	void initializeResult();
	void updateTitle();
	void updateGameMain();
	void updateResult();


	SceneManager::SceneState currentScene;
};

#endif 
