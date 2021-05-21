#pragma once
#include <cstdio>

#include "Position.h"
const int MAXLEN = 1000;
const int spacenum = 5;
inline void PrintLn(const char* sz) {
    printf("%s\n", sz);
    fflush(stdout);
}
bool judge(const char* arr, int n);
bool gettime(const char* arr, int n, int& time);
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
int PosToLine(int Pos, bool sdPlayer);
void BestMoveIntToNum(PositionStruct& pos, int bestmove);
int ChangeToFormatMove(int k);
