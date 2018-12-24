//����������������������������������������
// XorShift.cpp
//����������������������������������������

// ===== �C���N���[�h�� =====
#include "MyRandom.h"
#include <ctime>

MyRandom MyRandom::instance;
UINT MyRandom::seed;

//��������������������������
// �R���X�g���N�^
//��������������������������
MyRandom::MyRandom()
{
	setSeed(static_cast<UINT>((UINT)std::time(nullptr)));
}

//��������������������������
// �f�X�g���N�^
//��������������������������
MyRandom::~MyRandom()
{

}

//��������������������������
// �V�[�h
//��������������������������
void MyRandom::setSeed(UINT setSeed /*= 88675123*/)
{
	seed = setSeed;
}

//������������������������������������
// 0�`2 ^ 32 - 1�͈̗̔͂����𐶐�
//������������������������������������
UINT MyRandom::get()
{
	seed ^= (seed << 13);
	seed ^= (seed >> 17);
	seed ^= (seed << 15);
	return seed;
}