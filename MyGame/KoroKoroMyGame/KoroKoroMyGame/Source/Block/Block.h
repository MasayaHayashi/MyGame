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
#include <vector>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方定義 =====
class C_COLLIDER;

// ===== 列挙体定義 =====

// ===== 構造体定義 =====

// ===== クラス定義 =====
class Block final : public Pawn
{
public:
	Block();
	Block(UINT);
	~Block();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	void SetSelectNum(UINT);	// 現在の選択番号
//	EXPORT_BLOCK_DATA*	GetExportData();			// 書き出し用データ取得
//	void SetExportData(EXPORT_BLOCK_DATA SetData);	// 出力用データセット

protected:
	//void UpdateExportData();					// 出力用データ更新

//	EXPORT_BLOCK_DATA	ExportData;				// 書き出す際の保存用データ
//	BLOCK_TYPE			BlockType;
//	C_COLLIDER		    *pCollider;
	UINT				uCurrentSelect;
private:


	const std::string ModelPass		= "Data/Model/Block/box.x";
	const std::string TexturePass	= "Data/Texture/IceStone.png";


};

#endif
