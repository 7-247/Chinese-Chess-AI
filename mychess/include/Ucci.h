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

//����Ϊ������ʦ�ļ������뷽�����������62.5���ֵ�
inline int PosToLine(int Pos, bool sdPlayer) {
    if (!sdPlayer)
        return 9 - (Pos - 1) % 9;
    else
        return 1 + (Pos - 1) % 9;
}
inline void BestMoveIntToNum(PositionStruct& pos, int bestmove) {
    //������ֱ���Ͻ���ʱ����ʾ���ӽ��˵Ĳ�����������ƽ�߻�б�ߵ�ʱ�򣬱�ʾ������ֱ�ߵı��
    int low = (bestmove & 255), high = ((bestmove - low) >> 8);
    int type = 0;
    int highno = PosToNo(high, pos.ucsqPieces);
    printf("%d", type = cnPieceTypes[highno] + 1);
    if (type > 1 && type < 7) {
        int no_a = FROM_NUM[type - 1], no_b = FROM_NUM[type - 1] + 1;
        int pos_other = pos.ucsqPieces[no_a + no_b - highno];  //��һ����λ��
        if (PosToLine(pos_other, pos.sdPlayer) ==
            PosToLine(high, pos.sdPlayer)) {
            int highcol = 9 - (high - 1) / 9;
            int othercol = 9 - (pos_other - 1) / 9;
            if (pos.sdPlayer) {
                if (othercol > highcol)
                    printf("+");
                else
                    printf("-");
            } else {
                if (othercol > highcol)
                    printf("-");
                else
                    printf("+");
            }
        } else
            printf("%d", PosToLine(high, pos.sdPlayer));
    } else
        printf("%d", PosToLine(high, pos.sdPlayer));

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
    if (ch == '.' || type == 4 || type == 2 || type == 3)
        if (!pos.sdPlayer)
            printf("%d", 9 - (low - 1) % 9);  //������ƽ
        else
            printf("%d", 1 + (low - 1) % 9);
    else {
        printf("%d", abs(firstmoveline - nextmoveline));
    }
    printf("\n");
}
