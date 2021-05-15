#pragma once
#include <cstdio>
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
