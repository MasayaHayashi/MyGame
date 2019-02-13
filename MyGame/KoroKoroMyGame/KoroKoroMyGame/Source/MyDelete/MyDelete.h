//����������������������������������������������������������������
// Delete.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MY_DELETE_H
#define MY_DELETE_H

// ===== �C���N���[�h�� =====

// ===== �萔�E�}�N����` =====

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
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
