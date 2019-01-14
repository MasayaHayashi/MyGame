//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Block.h
// ブロッククラス(基底クラス)
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef BLOCK_H
#define BLOCK_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"
#include "../GameObjectBase/GameObjectBase.h"
#include <map>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方定義 =====
class C_COLLIDER;

// ===== 列挙体定義 =====

// ===== 構造体定義 =====

// ===== クラス定義 =====
class MainObject final : public Pawn
{
public:
	MainObject();
	MainObject(std::string, std::string,size_t,bool);
	~MainObject();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	const std::string getKeyName();

protected:
	
private:

	std::map<std::string, std::string>	modelPasses;
	std::map<std::string, std::string>	texturePasses;

	const std::string passName = "Data/Model/Block/";


//	const std::string ModelPass		= "Data/Model/Block/flatAndHill.x";

};

#endif
