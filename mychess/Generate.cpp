
#include <iostream>

#include "Position.h"

bool PositionStruct::judge9(int move_new) {
    //目标位置是否在9宫格中
    if (!sdPlayer)
        if (((move_new <= 87 && move_new >= 85) ||
             (move_new <= 78 && move_new >= 76) ||
             (move_new <= 69 && move_new >= 67)))
            return true;
        else
            return false;
    else {
        if (((move_new <= 6 && move_new >= 4) ||
             (move_new <= 15 && move_new >= 13) ||
             (move_new <= 24 && move_new >= 22)))
            return true;
        else
            return false;
    }
}
bool PositionStruct::judgeRiver(int move_new) {
    //目标位置是否在己方界中
    if (!sdPlayer) {
        if (move_new > 45)
            return true;
        else
            return false;
    } else {
        if (move_new <= 45)
            return true;
        else
            return false;
    }
}
bool judgeIn(int move_new) {
    //目标位置是否在棋盘中
    return move_new >= 1 && move_new <= 90;
}
bool PositionStruct::exist_friend(const int chessboard[], int pos, int move) {
    //目标位置是否有友方棋子
    return (chessboard[ucsqPieces[pos] + move] <= 7 + sdPlayer * 7 &&
            chessboard[ucsqPieces[pos] + move] >= 1 + sdPlayer * 7);
}
bool PositionStruct::exist_enemy(const int chessboard[], int pos, int move) {
    //目标位置是否有敌方棋子
    return (chessboard[ucsqPieces[pos] + move] <= 7 + (!sdPlayer) * 7 &&
            chessboard[ucsqPieces[pos] + move] >= 1 + (!sdPlayer) * 7);
}
bool PositionStruct::exist_all(const int chessboard[], int pos, int move) {
    //目标位置是否有棋子
    return (chessboard[ucsqPieces[pos] + move] <= 14 &&
            chessboard[ucsqPieces[pos] + move] >= 1);
}
bool PositionStruct::chief_rule(const int chessboard[], int pos, int move) {
    return judge9(ucsqPieces[pos] + move) &&
           !exist_friend(chessboard, pos, move);
}
vector<int> PositionStruct::move_chief(const int chessboard[], int pos) {
    //将帅的移动规则，pos表示当前棋子序数(0-31)
    vector<int> possible_move;
    if (chief_rule(chessboard, pos, 1))
        possible_move.push_back(ucsqPieces[pos] + 1);
    if (chief_rule(chessboard, pos, -1))
        possible_move.push_back(ucsqPieces[pos] - 1);
    if (chief_rule(chessboard, pos, 9))
        possible_move.push_back(ucsqPieces[pos] + 9);
    if (chief_rule(chessboard, pos, -9))
        possible_move.push_back(ucsqPieces[pos] - 9);
    int step = sdPlayer ? 9 : -9;
    for (int i = step;; i += step) {
        if (exist_all(chessboard, pos, i)) {
            if (chessboard[ucsqPieces[pos] + i] == 1 + (!sdPlayer) * 7)
                possible_move.push_back(ucsqPieces[pos] + i);
            break;
        }
    }
    return possible_move;
}
bool PositionStruct::knight_rule(const int chessboard[], int pos, int move) {
    return judge9(ucsqPieces[pos] + move) &&
           !exist_friend(chessboard, pos, move);
}
vector<int> PositionStruct::move_knight(const int chessboard[], int pos) {
    //士的移动规则，pos表示当前棋子序数(0-31)
    vector<int> possible_move;
    if (knight_rule(chessboard, pos, -8))
        possible_move.push_back(ucsqPieces[pos] - 8);
    if (knight_rule(chessboard, pos, -10))
        possible_move.push_back(ucsqPieces[pos] - 10);
    if (knight_rule(chessboard, pos, 8))
        possible_move.push_back(ucsqPieces[pos] + 8);
    if (knight_rule(chessboard, pos, 10))
        possible_move.push_back(ucsqPieces[pos] + 10);
    return possible_move;
}
bool PositionStruct::elephant_rule(const int chessboard[], int pos, int move) {
    return judgeRiver(ucsqPieces[pos] + move) &&
           judgeIn(ucsqPieces[pos] + move) &&
           !exist_friend(chessboard, pos, move) &&
           !exist_all(chessboard, pos, move / 2);
}
vector<int> PositionStruct::move_elephant(const int chessboard[], int pos) {
    vector<int> possible_move;
    if (ucsqPieces[pos] % 9 != 0 && elephant_rule(chessboard, pos, -16))
        possible_move.push_back(ucsqPieces[pos] - 16);
    if (ucsqPieces[pos] % 9 != 1 && elephant_rule(chessboard, pos, 16))
        possible_move.push_back(ucsqPieces[pos] + 16);
    if (ucsqPieces[pos] % 9 != 1 && elephant_rule(chessboard, pos, -20))
        possible_move.push_back(ucsqPieces[pos] - 20);
    if (ucsqPieces[pos] % 9 != 0 && elephant_rule(chessboard, pos, 20))
        possible_move.push_back(ucsqPieces[pos] + 20);
    return possible_move;
}
bool PositionStruct::horse_rule(const int chessboard[], int pos, int move) {
    return judgeIn(ucsqPieces[pos] + move) &&
           !exist_friend(chessboard, pos, move);
}
vector<int> PositionStruct::move_horse(const int chessboard[], int pos) {
    vector<int> possible_move;
    if (!exist_all(chessboard, pos, -9)) {
        if (ucsqPieces[pos] % 9 != 0 && horse_rule(chessboard, pos, -17))
            possible_move.push_back(ucsqPieces[pos] - 17);
        if (ucsqPieces[pos] % 9 != 1 && horse_rule(chessboard, pos, -19))
            possible_move.push_back(ucsqPieces[pos] - 19);
    }
    if (!exist_all(chessboard, pos, 9)) {
        if (ucsqPieces[pos] % 9 != 1 && horse_rule(chessboard, pos, 17))
            possible_move.push_back(ucsqPieces[pos] + 17);
        if (ucsqPieces[pos] % 9 != 0 && horse_rule(chessboard, pos, 19))
            possible_move.push_back(ucsqPieces[pos] + 19);
    }
    if (ucsqPieces[pos] % 9 != 1 && ucsqPieces[pos] % 9 != 2 &&
        !exist_all(chessboard, pos, -1)) {
        if (horse_rule(chessboard, pos, -11))
            possible_move.push_back(ucsqPieces[pos] - 11);
        if (horse_rule(chessboard, pos, 7))
            possible_move.push_back(ucsqPieces[pos] + 7);
    }
    if (ucsqPieces[pos] % 9 != 0 && ucsqPieces[pos] % 9 != 8 &&
        !exist_all(chessboard, pos, 1)) {
        if (horse_rule(chessboard, pos, 11))
            possible_move.push_back(ucsqPieces[pos] + 11);
        if (horse_rule(chessboard, pos, -7))
            possible_move.push_back(ucsqPieces[pos] - 7);
    }
    return possible_move;
}
vector<int> PositionStruct::move_cart(const int chessboard[], int pos) {
    vector<int> possible_move;
    if (ucsqPieces[pos] % 9 != 1)
        for (int i = -1;; i--) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (!exist_friend(chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                if (exist_all(chessboard, pos, i)) break;
            } else
                break;
        }
    if (ucsqPieces[pos] % 9 != 0)
        for (int i = 1;; i++) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (!exist_friend(chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                if (exist_all(chessboard, pos, i)) break;
            } else
                break;
        }
    for (int i = -9;; i -= 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (!exist_friend(chessboard, pos, i))
                possible_move.push_back(ucsqPieces[pos] + i);
            if (exist_all(chessboard, pos, i)) break;
        } else
            break;
    }
    for (int i = 9;; i += 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (!exist_friend(chessboard, pos, i))
                possible_move.push_back(ucsqPieces[pos] + i);
            if (exist_all(chessboard, pos, i)) break;
        } else
            break;
    }
    return possible_move;
}
vector<int> PositionStruct::move_cannon(const int chessboard[], int pos) {
    vector<int> possible_move;
    bool flag = 1;  // flag为0表示存在炮台了
    for (int i = -1;; i--) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (flag) {
                if (!exist_all(chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //有炮台，找敌方棋子
                if (exist_enemy(chessboard, pos, i)) {
                    flag = 1;
                    possible_move.push_back(ucsqPieces[pos] + i);
                    break;
                }
            }
            if ((ucsqPieces[pos] + i) % 9 == 1) break;
        } else
            break;
    }
    flag = 1;
    for (int i = 1;; i++) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (flag) {
                if (!exist_all(chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //有炮台，找敌方棋子
                if (exist_enemy(chessboard, pos, i)) {
                    flag = 1;
                    possible_move.push_back(ucsqPieces[pos] + i);
                    break;
                }
            }
            if ((ucsqPieces[pos] + i) % 9 == 0) break;
        } else
            break;
    }
    flag = 1;
    for (int i = -9;; i -= 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (flag) {
                if (!exist_all(chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //有炮台，找敌方棋子
                if (exist_enemy(chessboard, pos, i)) {
                    flag = 1;
                    possible_move.push_back(ucsqPieces[pos] + i);
                    break;
                }
            }
        } else
            break;
    }
    flag = 1;
    for (int i = 9;; i += 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (flag) {
                if (!exist_all(chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //有炮台，找敌方棋子
                if (exist_enemy(chessboard, pos, i)) {
                    flag = 1;
                    possible_move.push_back(ucsqPieces[pos] + i);
                    break;
                }
            }
        } else
            break;
    }
    return possible_move;
}
vector<int> PositionStruct::move_soldier(const int chessboard[], int pos) {
    vector<int> possible_move;
    if (judgeRiver(ucsqPieces[pos])) {
        int move = 9 * (sdPlayer ? 1 : -1);
        if (!exist_friend(chessboard, pos, move))
            possible_move.push_back(ucsqPieces[pos] + move);
    } else {
        int move = 9 * (sdPlayer ? 1 : -1);
        if (!exist_friend(chessboard, pos, move))
            possible_move.push_back(ucsqPieces[pos] + move);
        if (ucsqPieces[pos] % 9 != 0 && !exist_friend(chessboard, pos, 1))
            possible_move.push_back(ucsqPieces[pos] + 1);
        if (ucsqPieces[pos] % 9 != 1 && !exist_friend(chessboard, pos, -1))
            possible_move.push_back(ucsqPieces[pos] - 1);
    }
    return possible_move;
}

vector<vector<int>> PositionStruct::possibleMove(const int chessboard[]) {
    vector<int> (PositionStruct::*move_chess[7])(const int chessboard[],
                                                 int pos) = {
        &PositionStruct::move_chief,    &PositionStruct::move_knight,
        &PositionStruct::move_elephant, &PositionStruct::move_horse,
        &PositionStruct::move_cart,     &PositionStruct::move_cannon,
        &PositionStruct::move_soldier};
    vector<vector<int>> possible(
        32,
        vector<int>(
            1,
            0));  //所有棋子可能走法先全部置0，这可能会与被吃棋子的置0有冲突，读取方法应与下方循环相同
    for (int i = sdPlayer * 16; i < sdPlayer * 16 + 16; i++) {
        if (ucsqPieces[i] != 0)
            possible[i] = (this->*move_chess[cnPieceTypes[i]])(chessboard, i);
    }

    //请注释掉
    /*for (int i = 0; i < 32; i++) {  //输出
        cout << sdPlayer << "---" << cnPieceTypes[i] << "---" << ucsqPieces[i]
             << "---";
        for (int j = 0; j < possible[i].size(); j++)
            cout << possible[i][j] << " ";
        cout << endl;
    }*/

    return possible;
}

vector<vector<int>> PositionStruct::Generate() {
    int chessboard[91] = {0};
    for (int i = 0; i < 16; i++)
        chessboard[ucsqPieces[i]] = cnPieceTypes[i] + 1;
    for (int i = 16; i < 32; i++)
        chessboard[ucsqPieces[i]] = cnPieceTypes[i] + 1 + 7;

    return possibleMove(chessboard);
}

//将帅相对 没写
