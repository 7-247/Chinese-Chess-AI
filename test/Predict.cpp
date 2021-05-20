#include "Position.h"
static bool judge9(int sdPlayer, int move_new) {
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
static bool judgeRiver(int sdPlayer, int move_new) {
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
static bool judgeIn(int move_new) {
    //目标位置是否在棋盘中
    return move_new >= 1 && move_new <= 90;
}
static bool exist_all(const int ucsqPieces[], const int chessboard[], int pos,
                      int move) {
    //目标位置是否有棋子
    return (chessboard[ucsqPieces[pos] + move] <= 14 &&
            chessboard[ucsqPieces[pos] + move] >= 1);
}
static bool chief_rule(int sdPlayer, const int ucsqPieces[],
                       const int chessboard[], int pos, int move) {
    return judge9(sdPlayer, ucsqPieces[pos] + move);
}
static vector<int> move_chief(int sdPlayer, const int ucsqPieces[],
                              const int chessboard[], int pos) {
    //将帅的移动规则，pos表示当前棋子序数(0-31)
    vector<int> possible_move;
    int step = sdPlayer ? 9 : -9;
    for (int i = step;; i += step) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (exist_all(ucsqPieces, chessboard, pos, i)) {
                if (chessboard[ucsqPieces[pos] + i] == 1 + (!sdPlayer) * 7)
                    possible_move.push_back(ucsqPieces[pos] + i);
                break;
            }
        } else
            break;
    }
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, 1))
        possible_move.push_back(ucsqPieces[pos] + 1);
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, -1))
        possible_move.push_back(ucsqPieces[pos] - 1);
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, 9))
        possible_move.push_back(ucsqPieces[pos] + 9);
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, -9))
        possible_move.push_back(ucsqPieces[pos] - 9);
    return possible_move;
}
static bool knight_rule(int sdPlayer, const int ucsqPieces[],
                        const int chessboard[], int pos, int move) {
    return judge9(sdPlayer, ucsqPieces[pos] + move);
}
static vector<int> move_knight(int sdPlayer, const int ucsqPieces[],
                               const int chessboard[], int pos) {
    //士的移动规则，pos表示当前棋子序数(0-31)
    vector<int> possible_move;
    if (knight_rule(sdPlayer, ucsqPieces, chessboard, pos, -8))
        possible_move.push_back(ucsqPieces[pos] - 8);
    if (knight_rule(sdPlayer, ucsqPieces, chessboard, pos, -10))
        possible_move.push_back(ucsqPieces[pos] - 10);
    if (knight_rule(sdPlayer, ucsqPieces, chessboard, pos, 8))
        possible_move.push_back(ucsqPieces[pos] + 8);
    if (knight_rule(sdPlayer, ucsqPieces, chessboard, pos, 10))
        possible_move.push_back(ucsqPieces[pos] + 10);
    return possible_move;
}
static bool elephant_rule(int sdPlayer, const int ucsqPieces[],
                          const int chessboard[], int pos, int move) {
    return judgeRiver(sdPlayer, ucsqPieces[pos] + move) &&
           judgeIn(ucsqPieces[pos] + move) &&
           judgeIn(ucsqPieces[pos] + move / 2) &&
           !exist_all(ucsqPieces, chessboard, pos, move / 2);
}
static vector<int> move_elephant(int sdPlayer, const int ucsqPieces[],
                                 const int chessboard[], int pos) {
    vector<int> possible_move;
    if (ucsqPieces[pos] % 9 != 0 &&
        elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, -16))
        possible_move.push_back(ucsqPieces[pos] - 16);
    if (ucsqPieces[pos] % 9 != 1 &&
        elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, 16))
        possible_move.push_back(ucsqPieces[pos] + 16);
    if (ucsqPieces[pos] % 9 != 1 &&
        elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, -20))
        possible_move.push_back(ucsqPieces[pos] - 20);
    if (ucsqPieces[pos] % 9 != 0 &&
        elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, 20))
        possible_move.push_back(ucsqPieces[pos] + 20);
    return possible_move;
}
static bool horse_rule(int sdPlayer, const int ucsqPieces[],
                       const int chessboard[], int pos, int move) {
    return judgeIn(ucsqPieces[pos] + move);
}
static vector<int> move_horse(int sdPlayer, const int ucsqPieces[],
                              const int chessboard[], int pos) {
    vector<int> possible_move;
    if (judgeIn(ucsqPieces[pos] - 9) &&
        !exist_all(ucsqPieces, chessboard, pos, -9)) {
        if (ucsqPieces[pos] % 9 != 0 && judgeIn(ucsqPieces[pos] - 17) &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -17))
            possible_move.push_back(ucsqPieces[pos] - 17);
        if (ucsqPieces[pos] % 9 != 1 && judgeIn(ucsqPieces[pos] - 19) &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -19))
            possible_move.push_back(ucsqPieces[pos] - 19);
    }
    if (judgeIn(ucsqPieces[pos] + 9) &&
        !exist_all(ucsqPieces, chessboard, pos, 9)) {
        if (ucsqPieces[pos] % 9 != 1 && judgeIn(ucsqPieces[pos] + 17) &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, 17))
            possible_move.push_back(ucsqPieces[pos] + 17);
        if (ucsqPieces[pos] % 9 != 0 && judgeIn(ucsqPieces[pos] + 19) &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, 19))
            possible_move.push_back(ucsqPieces[pos] + 19);
    }
    if (judgeIn(ucsqPieces[pos] - 1) && ucsqPieces[pos] % 9 != 1 &&
        ucsqPieces[pos] % 9 != 2 &&
        !exist_all(ucsqPieces, chessboard, pos, -1)) {
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -11) &&
            judgeIn(ucsqPieces[pos] - 11))
            possible_move.push_back(ucsqPieces[pos] - 11);
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, 7) &&
            judgeIn(ucsqPieces[pos] + 7))
            possible_move.push_back(ucsqPieces[pos] + 7);
    }
    if (judgeIn(ucsqPieces[pos] + 1) && ucsqPieces[pos] % 9 != 0 &&
        ucsqPieces[pos] % 9 != 8 &&
        !exist_all(ucsqPieces, chessboard, pos, 1)) {
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, +11) &&
            judgeIn(ucsqPieces[pos] + 11))
            possible_move.push_back(ucsqPieces[pos] + 11);
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -7) &&
            judgeIn(ucsqPieces[pos] - 7))
            possible_move.push_back(ucsqPieces[pos] - 7);
    }
    return possible_move;
}
static vector<int> move_cart(int sdPlayer, const int ucsqPieces[],
                             const int chessboard[], int pos) {
    vector<int> possible_move;
    if (ucsqPieces[pos] % 9 != 1)
        for (int i = -1;; i--) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (!exist_all(ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                if (exist_all(ucsqPieces, chessboard, pos, i) ||
                    (ucsqPieces[pos] + i) % 9 == 1) {
                    possible_move.push_back(ucsqPieces[pos] + i);
                    break;
                }
            } else
                break;
        }
    if (ucsqPieces[pos] % 9 != 0)
        for (int i = 1;; i++) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (!exist_all(ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                if (exist_all(ucsqPieces, chessboard, pos, i) ||
                    ((ucsqPieces[pos] + i) % 9 == 0)) {
                    possible_move.push_back(ucsqPieces[pos] + i);
                    break;
                }
            } else
                break;
        }
    for (int i = -9;; i -= 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (!exist_all(ucsqPieces, chessboard, pos, i))
                possible_move.push_back(ucsqPieces[pos] + i);
            if (exist_all(ucsqPieces, chessboard, pos, i)) {
                possible_move.push_back(ucsqPieces[pos] + i);
                break;
            }
        } else
            break;
    }
    for (int i = 9;; i += 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (!exist_all(ucsqPieces, chessboard, pos, i))
                possible_move.push_back(ucsqPieces[pos] + i);
            if (exist_all(ucsqPieces, chessboard, pos, i)) {
                possible_move.push_back(ucsqPieces[pos] + i);
                break;
            }
        } else
            break;
    }
    return possible_move;
}
static vector<int> move_cannon(int sdPlayer, const int ucsqPieces[],
                               const int chessboard[], int pos) {
    vector<int> possible_move;
    bool flag = 1;  // flag为0表示存在炮台了
    if (ucsqPieces[pos] % 9 != 1)
        for (int i = -1;; i--) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (flag) {
                    if (!exist_all(ucsqPieces, chessboard, pos, i))
                        possible_move.push_back(ucsqPieces[pos] + i);
                    else
                        flag = 0;
                } else {  //有炮台，找敌方棋子
                    if (exist_all(ucsqPieces, chessboard, pos, i)) {
                        flag = 1;
                        possible_move.push_back(ucsqPieces[pos] + i);
                        break;
                    }
                }
            } else
                break;
            if ((ucsqPieces[pos] + i) % 9 == 1) break;
        }
    flag = 1;
    if (ucsqPieces[pos] % 9 != 0)
        for (int i = 1;; i++) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (flag) {
                    if (!exist_all(ucsqPieces, chessboard, pos, i))
                        possible_move.push_back(ucsqPieces[pos] + i);
                    else
                        flag = 0;
                } else {  //有炮台，找敌方棋子
                    if (exist_all(ucsqPieces, chessboard, pos, i)) {
                        flag = 1;
                        possible_move.push_back(ucsqPieces[pos] + i);
                        break;
                    }
                }
            } else
                break;
            if ((ucsqPieces[pos] + i) % 9 == 0) break;
        }
    flag = 1;
    for (int i = -9;; i -= 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (flag) {
                if (!exist_all(ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //有炮台，找敌方棋子
                if (exist_all(ucsqPieces, chessboard, pos, i)) {
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
                if (!exist_all(ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //有炮台，找敌方棋子
                if (exist_all(ucsqPieces, chessboard, pos, i)) {
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
static vector<int> move_soldier(int sdPlayer, const int ucsqPieces[],
                                const int chessboard[], int pos) {
    vector<int> possible_move;
    if (judgeRiver(sdPlayer, ucsqPieces[pos])) {
        int move = 9 * (sdPlayer ? 1 : -1);
        if (judgeIn(ucsqPieces[pos] + move))
            possible_move.push_back(ucsqPieces[pos] + move);
    } else {
        int move = 9 * (sdPlayer ? 1 : -1);
        if (judgeIn(ucsqPieces[pos] + move))
            possible_move.push_back(ucsqPieces[pos] + move);
        if (judgeIn(ucsqPieces[pos] + 1) && ucsqPieces[pos] % 9 != 0)
            possible_move.push_back(ucsqPieces[pos] + 1);
        if (judgeIn(ucsqPieces[pos] - 1) && ucsqPieces[pos] % 9 != 1)
            possible_move.push_back(ucsqPieces[pos] - 1);
    }
    return possible_move;
}

static inline vector<vector<int>> possibleMove(int sdPlayer,
                                               const int ucsqPieces[],
                                               const int chessboard[]) {
    vector<int> (*move_chess[7])(int sdPlayer, const int ucsqPieces[],
                                 const int chessboard[], int pos) = {
        move_chief, move_knight, move_elephant, move_horse,
        move_cart,  move_cannon, move_soldier};
    vector<vector<int>> possible(
        32,
        vector<int>(
            1,
            0));  //所有棋子可能走法先全部置0，这可能会与被吃棋子的置0有冲突，读取方法应与下方循环相同
    for (int i = sdPlayer * 16; i < sdPlayer * 16 + 16; i++) {
        if (ucsqPieces[i] != 0)
            possible[i] = move_chess[cnPieceTypes[i]](sdPlayer, ucsqPieces,
                                                      chessboard, i);
    }

    return possible;
}
vector<vector<int>> PositionStruct::Predict() {
    int chessboard[91] = {0};
    for (int i = 0; i < 16; i++)
        chessboard[ucsqPieces[i]] = cnPieceTypes[i] + 1;
    for (int i = 16; i < 32; i++)
        chessboard[ucsqPieces[i]] = cnPieceTypes[i] + 1 + 7;

    return possibleMove(sdPlayer, ucsqPieces, chessboard);
}
