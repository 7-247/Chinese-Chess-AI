#include "Ucci.h"

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
//����Ϊ������ʦ�ļ������뷽�����������62.5���ֵ�
inline int PosToLine(int Pos, bool sdPlayer) {
    if (!sdPlayer)
        return 9 - (Pos - 1) % 9;
    else
        return 1 + (Pos - 1) % 9;
}
void BestMoveIntToNum(PositionStruct& pos, int bestmove) {
    //������ֱ���Ͻ���ʱ����ʾ���ӽ��˵Ĳ�����������ƽ�߻�б�ߵ�ʱ�򣬱�ʾ������ֱ�ߵı��
    int low = (bestmove & 255), high = ((bestmove - low) >> 8);
    int type = 0;
    int highno = PosToNo(high, pos.nowPos);
    printf("%d", type = cnPieceTypes[highno] + 1);
    if (type > 3 && type < 7) {
        int no_a = FROM_NUM[type - 1] + 16 * pos.sdPlayer,
            no_b = FROM_NUM[type - 1] + 1 + pos.sdPlayer * 16;
        int pos_other = pos.nowPos[no_a + no_b - highno];  //��һ����λ��
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

int ChangeToFormatMove(int move) {
    const char* k = (const char*)&move;
    int j = k[0] - 'a';
    int i = 9 - k[1] + '0';
    int highpos = i * 9 + j + 1;
    int nj = k[2] - 'a';
    int ni = 9 - k[3] + '0';
    int lowpos = ni * 9 + nj + 1;
    return (highpos << 8) + lowpos;
    // k[0],k[1],k[2],k[3]
}
