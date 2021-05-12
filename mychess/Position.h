#pragma once
#include <cstdio>
#include<cstring>

/* 
	������Ŷ�Ӧ����������
	0��15��ʾ���ӣ�16��31��ʾ���ӡ�ÿ��������˳�������ǣ�
	ÿ��������˳�������ǣ�˧�����������������ڱ���������(��ʿʿ������������������������)
	�ж������Ǻ�����"pc < 16"��������"pc >= 16"
 */
static const int cnPieceTypes[32] = {
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

// �������Ͷ�Ӧ�����ӷ���
const char* const cszPieceBytes = "KABNRCP";

const int FROM_NUM[] = { 0,1,3,5,7,9,11 };// ÿ�������Ŀ�ʼ��źͽ������

// FEN�������ӱ�ʶ��ע���������ֻ��ʶ���д��ĸ�������Сд��ĸʱ�����ȱ���ת��Ϊ��д
const int FenPiece[] = {
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	1,2,5,7,2,7,7,3,7,7,0,7,7,3,7,6,7,4 
};

// ��ʼ�����FEN��
const char* const cszStartFen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";

// ����ṹ
struct PositionStruct {
	int ucsqPieces[32];		// ÿ�������������Ϸŵ�λ�ã�0��ʾ������
	//����0��15��ʾ���ӣ�16��31��ʾ���ӡ�ÿ��������˳�������ǣ�
	//˧�����������������ڱ���������(��	ʿʿ������������������������)
	//�ж������Ǻ�����"pc < 16"��������"pc >= 16"
	//�����ֵΪ33~cb
	bool sdPlayer;                  // �ֵ�˭�ߣ�0=�췽��1=�ڷ�
	int RoundNum;					// �غ���
	PositionStruct() {
		FromFen(cszStartFen);
	}
	PositionStruct(const char* const arr) {
		FromFen(arr);
	}
	void ClearBoard();
	void FromFen(const char* const);
	void AddPiece(int, int); // �������Ϸ�һö����
	void DelPiece(int); // ������������һö����
};
inline int GetPiecePos(int i, int j);
