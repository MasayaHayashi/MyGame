//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Block.h
// ブロッククラス(基底クラス)
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef GOAL_H
#define GOAL_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"
#include "../GameObjectBase/GameObjectBase.h"
#include <map>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方定義 =====

// ===== 列挙体定義 =====

// ===== 構造体定義 =====

// ===== クラス定義 =====
class Goal final : public Pawn
{
public:
	Goal();
	Goal(std::string ,size_t);
	~Goal();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	const std::string Goal::getKeyName();

protected:
	
private:

	std::map<std::string, std::string> modelPasses;

	const std::string passName = "Data/Model/Goal/";

//	const std::string ModelPass		= "Data/Model/Goal/flatAndHill.x";
	const std::string TexturePass	= "Data/Texture/double_1.png";

};

#endif
