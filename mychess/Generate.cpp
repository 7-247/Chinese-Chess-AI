#include "Position.h"
bool judge9(int sdPlayer, int move_new) {
    //Ŀ��λ���Ƿ���9������
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
bool judgeRiver(int sdPlayer, int move_new) {
    //Ŀ��λ���Ƿ��ڼ�������
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
    //Ŀ��λ���Ƿ���������
    return move_new >= 1 && move_new <= 90;
}
bool exist_friend(int sdPlayer, const int ucsqPieces[], const int chessboard[],
                  int pos, int move) {
    //Ŀ��λ���Ƿ����ѷ�����
    return (chessboard[ucsqPieces[pos] + move] <= 7 + sdPlayer * 7 &&
            chessboard[ucsqPieces[pos] + move] >= 1 + sdPlayer * 7);
}
bool exist_enemy(int sdPlayer, const int ucsqPieces[], const int chessboard[],
                 int pos, int move) {
    //Ŀ��λ���Ƿ��ез�����
    return (chessboard[ucsqPieces[pos] + move] <= 7 + (!sdPlayer) * 7 &&
            chessboard[ucsqPieces[pos] + move] >= 1 + (!sdPlayer) * 7);
}
bool exist_all(const int ucsqPieces[], const int chessboard[], int pos,
               int move) {
    //Ŀ��λ���Ƿ�������
    return (chessboard[ucsqPieces[pos] + move] <= 14 &&
            chessboard[ucsqPieces[pos] + move] >= 1);
}
bool chief_rule(int sdPlayer, const int ucsqPieces[], const int chessboard[],
                int pos, int move) {
    return judge9(sdPlayer, ucsqPieces[pos] + move) &&
           !exist_friend(sdPlayer, ucsqPieces, chessboard, pos, move);
}
vector<int> move_chief(int sdPlayer, const int ucsqPieces[],
                       const int chessboard[], int pos) {
    //��˧���ƶ�����pos��ʾ��ǰ��������(0-31)
    vector<int> possible_move;
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, 1))
        possible_move.push_back(ucsqPieces[pos] + 1);
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, -1))
        possible_move.push_back(ucsqPieces[pos] - 1);
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, 9))
        possible_move.push_back(ucsqPieces[pos] + 9);
    if (chief_rule(sdPlayer, ucsqPieces, chessboard, pos, -9))
        possible_move.push_back(ucsqPieces[pos] - 9);

    int step = sdPlayer ? 9 : -9;
    for (int i = step;; i += step) {
        if (exist_all(ucsqPieces, chessboard, pos, i)) {
            if (chessboard[ucsqPieces[pos] + i] == 1 + (!sdPlayer) * 7)
                possible_move.push_back(ucsqPieces[pos] + i);
            break;
        }
    }
    return possible_move;
}
bool knight_rule(int sdPlayer, const int ucsqPieces[], const int chessboard[],
                 int pos, int move) {
    return judge9(sdPlayer, ucsqPieces[pos] + move) &&
           !exist_friend(sdPlayer, ucsqPieces, chessboard, pos, move);
}
vector<int> move_knight(int sdPlayer, const int ucsqPieces[],
                        const int chessboard[], int pos) {
    //ʿ���ƶ�����pos��ʾ��ǰ��������(0-31)
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
bool elephant_rule(int sdPlayer, const int ucsqPieces[], const int chessboard[],
                   int pos, int move) {
    return judgeRiver(sdPlayer, ucsqPieces[pos] + move) &&
           judgeIn(ucsqPieces[pos] + move) &&
           !exist_friend(sdPlayer, ucsqPieces, chessboard, pos, move) &&
           !exist_all(ucsqPieces, chessboard, pos, move / 2);
}
vector<int> move_elephant(int sdPlayer, const int ucsqPieces[],
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
bool horse_rule(int sdPlayer, const int ucsqPieces[], const int chessboard[],
                int pos, int move) {
    return judgeIn(ucsqPieces[pos] + move) &&
           !exist_friend(sdPlayer, ucsqPieces, chessboard, pos, move);
}
vector<int> move_horse(int sdPlayer, const int ucsqPieces[],
                       const int chessboard[], int pos) {
    vector<int> possible_move;
    if (!exist_all(ucsqPieces, chessboard, pos, -9)) {
        if (ucsqPieces[pos] % 9 != 0 &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -17))
            possible_move.push_back(ucsqPieces[pos] - 17);
        if (ucsqPieces[pos] % 9 != 1 &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -19))
            possible_move.push_back(ucsqPieces[pos] - 19);
    }
    if (!exist_all(ucsqPieces, chessboard, pos, 9)) {
        if (ucsqPieces[pos] % 9 != 1 &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, 17))
            possible_move.push_back(ucsqPieces[pos] + 17);
        if (ucsqPieces[pos] % 9 != 0 &&
            horse_rule(sdPlayer, ucsqPieces, chessboard, pos, 19))
            possible_move.push_back(ucsqPieces[pos] + 19);
    }
    if (ucsqPieces[pos] % 9 != 1 && ucsqPieces[pos] % 9 != 2 &&
        !exist_all(ucsqPieces, chessboard, pos, -1)) {
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -11))
            possible_move.push_back(ucsqPieces[pos] - 11);
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, 7))
            possible_move.push_back(ucsqPieces[pos] + 7);
    }
    if (ucsqPieces[pos] % 9 != 0 && ucsqPieces[pos] % 9 != 8 &&
        !exist_all(ucsqPieces, chessboard, pos, 1)) {
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, 11))
            possible_move.push_back(ucsqPieces[pos] + 11);
        if (horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -7))
            possible_move.push_back(ucsqPieces[pos] - 7);
    }
    return possible_move;
}
vector<int> move_cart(int sdPlayer, const int ucsqPieces[],
                      const int chessboard[], int pos) {
    vector<int> possible_move;
    if (ucsqPieces[pos] % 9 != 1)
        for (int i = -1;; i--) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (!exist_friend(sdPlayer, ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                if (exist_all(ucsqPieces, chessboard, pos, i)) break;
            } else
                break;
        }
    if (ucsqPieces[pos] % 9 != 0)
        for (int i = 1;; i++) {
            if (judgeIn(ucsqPieces[pos] + i)) {
                if (!exist_friend(sdPlayer, ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                if (exist_all(ucsqPieces, chessboard, pos, i)) break;
            } else
                break;
        }
    for (int i = -9;; i -= 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (!exist_friend(sdPlayer, ucsqPieces, chessboard, pos, i))
                possible_move.push_back(ucsqPieces[pos] + i);
            if (exist_all(ucsqPieces, chessboard, pos, i)) break;
        } else
            break;
    }
    for (int i = 9;; i += 9) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (!exist_friend(sdPlayer, ucsqPieces, chessboard, pos, i))
                possible_move.push_back(ucsqPieces[pos] + i);
            if (exist_all(ucsqPieces, chessboard, pos, i)) break;
        } else
            break;
    }
    return possible_move;
}
vector<int> move_cannon(int sdPlayer, const int ucsqPieces[],
                        const int chessboard[], int pos) {
    vector<int> possible_move;
    bool flag = 1;  // flagΪ0��ʾ������̨��
    for (int i = -1;; i--) {
        if (judgeIn(ucsqPieces[pos] + i)) {
            if (flag) {
                if (!exist_all(ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //����̨���ҵз�����
                if (exist_enemy(sdPlayer, ucsqPieces, chessboard, pos, i)) {
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
                if (!exist_all(ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //����̨���ҵз�����
                if (exist_enemy(sdPlayer, ucsqPieces, chessboard, pos, i)) {
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
                if (!exist_all(ucsqPieces, chessboard, pos, i))
                    possible_move.push_back(ucsqPieces[pos] + i);
                else
                    flag = 0;
            } else {  //����̨���ҵз�����
                if (exist_enemy(sdPlayer, ucsqPieces, chessboard, pos, i)) {
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
            } else {  //����̨���ҵз�����
                if (exist_enemy(sdPlayer, ucsqPieces, chessboard, pos, i)) {
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
vector<int> move_soldier(int sdPlayer, const int ucsqPieces[],
                         const int chessboard[], int pos) {
    vector<int> possible_move;
    if (judgeRiver(sdPlayer, ucsqPieces[pos])) {
        int move = 9 * (sdPlayer ? 1 : -1);
        if (!exist_friend(sdPlayer, ucsqPieces, chessboard, pos, move))
            possible_move.push_back(ucsqPieces[pos] + move);
    } else {
        int move = 9 * (sdPlayer ? 1 : -1);
        if (!exist_friend(sdPlayer, ucsqPieces, chessboard, pos, move))
            possible_move.push_back(ucsqPieces[pos] + move);
        if (ucsqPieces[pos] % 9 != 0 &&
            !exist_friend(sdPlayer, ucsqPieces, chessboard, pos, 1))
            possible_move.push_back(ucsqPieces[pos] + 1);
        if (ucsqPieces[pos] % 9 != 1 &&
            !exist_friend(sdPlayer, ucsqPieces, chessboard, pos, -1))
            possible_move.push_back(ucsqPieces[pos] - 1);
    }
    return possible_move;
}

vector<vector<int>> possibleMove(int sdPlayer, const int ucsqPieces[],
                                 const int chessboard[]) {
    vector<int> (*move_chess[7])(int sdPlayer, const int ucsqPieces[],
                                 const int chessboard[], int pos) = {
        move_chief, move_knight, move_elephant, move_horse,
        move_cart,  move_cannon, move_soldier};
    vector<vector<int>> possible(
        32,
        vector<int>(
            1,
            0));  //�������ӿ����߷���ȫ����0������ܻ��뱻�����ӵ���0�г�ͻ����ȡ����Ӧ���·�ѭ����ͬ
    for (int i = sdPlayer * 16; i < sdPlayer * 16 + 16; i++) {
        if (ucsqPieces[i] != 0)
            possible[i] = move_chess[cnPieceTypes[i]](sdPlayer, ucsqPieces,
                                                      chessboard, i);
    }

    return possible;
}
vector<vector<int>> PositionStruct::Generate() {
    int chessboard[91] = {0};
    for (int i = 0; i < 16; i++)
        chessboard[ucsqPieces[i]] = cnPieceTypes[i] + 1;
    for (int i = 16; i < 32; i++)
        chessboard[ucsqPieces[i]] = cnPieceTypes[i] + 1 + 7;

    return possibleMove(sdPlayer, ucsqPieces, chessboard);
}
