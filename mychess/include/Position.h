#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

/*
    ������Ŷ�Ӧ����������
    0��15��ʾ���ӣ�16��31��ʾ���ӡ�ÿ��������˳�������ǣ�
    ÿ��������˳�������ǣ�˧�����������������ڱ���������(��ʿʿ������������������������)
    �ж������Ǻ�����"pc < 16"��������"pc >= 16"
 */
static const int cnPieceTypes[32] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
                                     6, 6, 6, 6, 6, 0, 1, 1, 2, 2, 3,
                                     3, 4, 4, 5, 5, 6, 6, 6, 6, 6};

// �������Ͷ�Ӧ�����ӷ���
const char* const cszPieceBytes = "KABNRCP";

const int FROM_NUM[] = {0, 1, 3, 5, 7, 9, 11};  // ÿ�������Ŀ�ʼ��źͽ������

const char* const cszPieceBytesInChineseBlack[7] = {"��", "ʿ", "��", "�a",
                                                    "܇", "�h", "��"};
const char* const cszPieceBytesInChineseRed[7] = {"˧", "��", "��", "��",
                                                  "��", "��", "��"};

// FEN�������ӱ�ʶ��ע���������ֻ��ʶ���д��ĸ�������Сд��ĸʱ�����ȱ���ת��Ϊ��д
const int FenPiece[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1, 2, 5,
                        7, 2, 7, 7, 3, 7, 7, 0, 7, 7, 3, 7, 6, 7, 4};

// ���̷�Χ9*10
const int RANK_TOP = 0;
const int RANK_BOTTOM = 9;
const int FILE_LEFT = 0;
const int FILE_RIGHT = 8;
const int ROW = 10;
const int COL = 9;
// ��ʼ�����FEN��
const char* const cszStartFen =
    "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
struct moveNode {
    int src, dst;
    bool iseat;
    bool operator==(moveNode& node1);
};
// ����ṹ
struct PositionStruct {
    int ucsqPieces[32];  // ÿ�������������Ϸŵ�λ�ã�0��ʾ������
    //����0��15��ʾ���ӣ�16��31��ʾ���ӡ�ÿ��������˳�������ǣ�
    //˧�����������������ڱ���������(��ʿʿ������������������������)
    //�ж������Ǻ�����"pc < 16"��������"pc >= 16"
    //�����ֵΪ33~cb
    bool sdPlayer;  // �ֵ�˭�ߣ�0=�췽��1=�ڷ�
    int RoundNum;   // �غ���
    vector<moveNode> Moves;
    int Count;  // Moves[]β���ۻ��˶��ٸ�û���ֵ��߷�
    PositionStruct() { FromFen(cszStartFen); }
    PositionStruct(const char* const arr) { FromFen(arr); }
    void PositionInit(const char* const arr) { FromFen(arr); }
    void ClearBoard();
    void FromFen(const char* const);
    void AddPiece(int, int);         // �������Ϸ�һö����
    void DelPiece(int);              // ������������һö����
    vector<vector<int>> Generate();  //���㺯���������������߷�
    int Repeat();
    vector<vector<int>> Predict();  //���㺯���������������߷�(��������в)
};
inline void PositionStruct::DelPiece(int no) {  // ������������һö����
    ucsqPieces[no] = 0;
}
inline int PosToNo(int pos, int ucsqPieces[]) {
    for (int i = 0; i < 32; ++i)
        if (pos == ucsqPieces[i]) return i;
    return -1;
}
inline int GetPiecePos(int i, int j) {
    return i * COL + j + 1;  //�����е�0���ţ���1~90
}