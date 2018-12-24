//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// XorShift.cpp
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "MyRandom.h"
#include <ctime>

MyRandom MyRandom::instance;
UINT MyRandom::seed;

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝
MyRandom::MyRandom()
{
	setSeed(static_cast<UINT>((UINT)std::time(nullptr)));
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝
MyRandom::~MyRandom()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// シード
//＝＝＝＝＝＝＝＝＝＝＝＝＝
void MyRandom::setSeed(UINT setSeed /*= 88675123*/)
{
	seed = setSeed;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 0～2 ^ 32 - 1の範囲の乱数を生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
UINT MyRandom::get()
{
	seed ^= (seed << 13);
	seed ^= (seed >> 17);
	seed ^= (seed << 15);
	return seed;
}