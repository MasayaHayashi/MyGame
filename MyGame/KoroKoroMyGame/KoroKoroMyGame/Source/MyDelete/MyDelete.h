//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Delete.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MY_DELETE_H
#define MY_DELETE_H

// ===== インクルード部 =====

// ===== 定数・マクロ定義 =====

// ===== 構造体定義 =====

// ===== 列挙体定義 =====

// ===== クラス定義 =====
class Mydelete final
{
public:
	Mydelete();
	~Mydelete();

	template <class T>

	static inline void safeDelete(T *&p) 
	{
		delete p;
		p = nullptr;
	}

	template <class T>

	static inline void safeRelease(T *&p)
	{
		p->Release(); 
		p = nullptr;
	}


private:


};

#endif 
