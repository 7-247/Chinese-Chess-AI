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
    //棋子在直线上进退时，表示棋子进退的步数；当棋子平走或斜走的时候，表示所到达直线的编号
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
        printf("%d", 9 - (low - 1) % 9);  //仕相马、平
    else {
        printf("%d", abs(firstmoveline - nextmoveline));
    }
    printf("\n");
}
