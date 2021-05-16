#pragma once
#include <cstdlib>

#include "Position.h"
const int MAXLEN = 1000;
const int spacenum = 5;
inline void PrintLn(const char* sz) {
    printf("%s\n", sz);
    fflush(stdout);
}
bool judge(const char* arr, int n) {
    int num = 0;
    for (int i = 0; i < n && arr[i] && num < spacenum; i++) {
        num += arr[i] == ' ';
    }
    return num == spacenum;
}
bool gettime(const char* arr, int n, int& time) {
    time = 0;
    for (int i = 0; i < n && arr[i]; i++) {
        if (arr[i] >= '0' && arr[i] <= '9') {
            time = time * 10 + int(arr[i] - '0');
        } else
            return false;
    }
    return true;
}
inline void PosIntToChar(int Pos) {
    --Pos;
    printf("%c%d", Pos % 9 + 'a', 9 - Pos / 9);
}
inline void BestMoveIntToChar(int bestmove) {
    int low = bestmove & 255, high = (bestmove - low) >> 8;
    printf("bestmove ");
    PosIntToChar(high);
    PosIntToChar(low);
    printf("\n");
}
inline void BestMoveIntToNum(PositionStruct& pos, int bestmove) {
    //������ֱ���Ͻ���ʱ����ʾ���ӽ��˵Ĳ�����������ƽ�߻�б�ߵ�ʱ�򣬱�ʾ������ֱ�ߵı��
    int low = (bestmove & 255), high = ((bestmove - low) >> 8);
    int type = 0;
    printf("%d", type = cnPieceTypes[PosToNo(high, pos.ucsqPieces)] + 1);
    printf("%d", 9 - (high - 1) % 9);
    char ch = '.';
    int firstmoveline = 9 - (high - 1) / 9, nextmoveline = 9 - (low - 1) / 9;
    if (pos.sdPlayer == 0) {
        if (nextmoveline > firstmoveline)
            ch = '+';
        else if (nextmoveline < firstmoveline)
            ch = '-';
    } else {
        if (nextmoveline > firstmoveline)
            ch = '-';
        else if (nextmoveline < firstmoveline)
            ch = '+';
    }
    printf("%c", ch);
    if (ch == '.' || type == 1 || type == 2 || type == 3)
        printf("%d", 9 - (low - 1) % 9);  //������ƽ
    else {
        printf("%d", abs(firstmoveline - nextmoveline));
    }
    printf("\n");
}
