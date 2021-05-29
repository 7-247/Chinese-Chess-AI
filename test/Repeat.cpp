#include "Position.h"

//将军
bool readyForKill(int sdPlayer, const int* nowPos,
                  vector<vector<int>>& possible) {
    for (int i = 0 + sdPlayer * 16; i < 16 + sdPlayer * 16; i++) {
        for (int j = 0; j < possible[i].size(); j++) {
            if (possible[i][j] == nowPos[!sdPlayer * 16]) return true;
        }
    }
    return false;
}

//捉 hunter为进攻方棋子位置，target为防守方棋子位置
bool readyForCatch(int hunter, int target, const int* nowPos,
                   vector<vector<int>>& possible) {
    int pos = 0;
    for (int i = 0; i < 32; i++) {
        if (nowPos[i] == hunter) {
            pos = i;
            break;
        }
    }
    for (int i = 0; i < possible[pos].size(); i++)
        if (possible[pos][i] == target) return true;
    return false;
}

int PositionStruct::Repeat() {
    //红方长打则返回1（红方负）；黑方长打则返回2（黑方负）；双方都长打/重复不变判和返回0（平局）；没有重复局面则返回-1
    //你可能会用到的成员变量：Count,sdPlayer,Moves

    if (Count < 12) return -1;
    int length = Moves.size();
    vector<int> count(32, 0);
    for (int i = 0; i < 4; i++) {
        if (Moves[length - 1 - i] == Moves[length - 1 - i - 4] &&
            Moves[length - 1 - i] == Moves[length - 1 - i - 8]) {
            continue;
        } else {
            return -1;
        }
    }
    //连续重复局面
    int h_pos = 0, t_pos = 0;
    for (int i = 0; i < 32; i++) {
        if (nowPos[i] == Moves[length - 1].dst) {
            h_pos = i;
            break;
        }
    }
    for (int i = 0; i < 32; i++) {
        if (nowPos[i] == Moves[length - 2].dst) {
            t_pos = i;
            break;
        }
    }
    if (h_pos != 0 && h_pos != 16 && t_pos != 0 && t_pos != 16) {
        vector<vector<int>> possible = Generate();  //当前状态
        if (readyForKill(sdPlayer, nowPos, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 1].dst, Moves[length - 2].dst,
                          nowPos, possible))
            count[t_pos]++;

        nowPos[h_pos] = Moves[length - 1].src;  //最后一步回溯
        sdPlayer = !sdPlayer;
        possible = Generate();
        if (readyForKill(sdPlayer, nowPos, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 2].dst, Moves[length - 1].dst,
                          nowPos, possible))
            count[h_pos]++;

        nowPos[t_pos] = Moves[length - 2].src;  //倒数第二步回溯
        sdPlayer = !sdPlayer;
        possible = Generate();
        if (readyForKill(sdPlayer, nowPos, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 3].dst, Moves[length - 4].dst,
                          nowPos, possible))
            count[h_pos]++;

        nowPos[h_pos] = Moves[length - 3].src;  //倒数第三步回溯
        sdPlayer = !sdPlayer;
        possible = Generate();
        if (readyForKill(sdPlayer, nowPos, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 4].dst, Moves[length - 3].dst,
                          nowPos, possible))
            count[t_pos]++;
        //回复结构体内容
        sdPlayer = !sdPlayer;
        nowPos[h_pos] = Moves[length - 1].dst;
        nowPos[t_pos] = Moves[length - 2].dst;

        int black = 0, red = 0;
        if (h_pos > 16) {
            red = count[t_pos] + count[0];
            black = count[h_pos] + count[16];
        } else {
            red = count[h_pos] + count[0];
            black = count[t_pos] + count[16];
        }
        if (red == black)
            return 0;
        else if (red > black)
            return 2;
        else
            return 1;
    }

    if (h_pos == 16 || t_pos == 16) return 1;
    if (h_pos == 0 || t_pos == 0) return 2;
    return -1;
}
