//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MyRandom.h
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MYRANDOM_H
#define MYRANDOM_H

// ===== インクルード部 =====
#include "../DirectX3D/DirectX3D.h"

class MyRandom final
{
public :
	void setSeed(UINT seed = 88675123);
	static UINT get();

	template<class T>
	static T get(T min, T max)
	{	
		return (T)(get() % 0xFFFF) / 0xFFFF * (max - min) + min;
	}

private:
	static MyRandom instance;
	static UINT seed;

	MyRandom();
	~MyRandom();
};

#endif