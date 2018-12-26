//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SelectManager.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SELECT_MANAGER_H
#define SELECT_MANAGER_H

// ===== インクルード部 =====
#include <memory>
#include <map>

// ===== クラスの前方宣言 =====

// ===== クラス定義 =====
class SelectManager final
{
public:

	SelectManager();
	~SelectManager();

	static void initialize();
	static void update();
	static void updateFade();
	static void draw();
	static void drawFade();
	static void finalize();

protected:

private:
	static std::unique_ptr<SelectManager> SelectManagerInstancePtr;
};

#endif