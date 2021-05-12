#include <iostream>
#include <vector>
using namespace std;
/* 棋子序号对应的棋子类型
 * 棋子序号从0到15表示红子，16到31表示黑子。
 * 每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 提示：判断棋子是红子用"pc < 16"，黑子用"pc >= 16"
 */
static const int cnPieceTypes[32] = {
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6};
bool judge9(int sdPlayer, int move_new)
{
    if (!sdPlayer)
        if (((move_new <= 86 && move_new >= 84) ||
             (move_new <= 77 && move_new >= 75) ||
             (move_new <= 68 && move_new >= 66)))
            return true;
        else
            return false;
    else
    {
        if (((move_new <= 6 && move_new >= 4) ||
             (move_new <= 15 && move_new >= 13) ||
             (move_new <= 24 && move_new >= 22)))
            return true;
        else
            return false;
    }
}
bool judgeRiver(int sdPlayer, int move_new)
{
    if (!sdPlayer)
    {
        if (move_new > 45)
            return true;
        else
            return false;
    }
    else
    {
        if (move_new <= 45)
            return true;
        else
            return false;
    }
}
bool judgeIn(int move_new)
{
    return move_new >= 1 && move_new <= 90;
}
bool exist_friend(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos, int move)
{
    //目标位置是否有友方棋子
    return (chessboard[ucsqPieces[pos] + move] <= 6 + sdPlayer * 7 &&
            chessboard[ucsqPieces[pos] + move] >= 0 + sdPlayer * 7);
}
bool exist_all(const int (&ucsqPieces)[], const int (&chessboard)[], int pos, int move)
{
    //目标位置是否有棋子
    return (chessboard[ucsqPieces[pos] + move] <= 13 &&
            chessboard[ucsqPieces[pos] + move] >= 0);
}
bool chief_rule(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos, int move)
{
    return (judge9(sdPlayer, ucsqPieces[pos] + move) &&
            !(chessboard[ucsqPieces[pos] + move] <= 6 + sdPlayer * 7 &&
              chessboard[ucsqPieces[pos] + move] >= 0 + sdPlayer * 7));
}
vector<int> move_chief(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos)
{
    //将帅的移动规则，pos表示当前棋子序数(0-31)
    vector<int> possible_move;
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
bool knight_rule(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos, int move)
{
    return judge9(sdPlayer, ucsqPieces[pos] + move) &&
           !(chessboard[ucsqPieces[pos] + move] <= 6 + sdPlayer * 7 &&
             chessboard[ucsqPieces[pos] + move] >= 0 + sdPlayer * 7);
}
vector<int> move_knight(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos)
{
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
bool elephant_rule(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos, int move)
{
    return judgeRiver(sdPlayer, ucsqPieces[pos] + move) &&
           judgeIn(ucsqPieces[pos] + move) &&
           !(chessboard[ucsqPieces[pos] + move] <= 6 + sdPlayer * 7 &&
             chessboard[ucsqPieces[pos] + move] >= 0 + sdPlayer * 7) &&
           !(chessboard[ucsqPieces[pos] + move / 2] <= 13 &&
             chessboard[ucsqPieces[pos] + move / 2] >= 0);
}
vector<int> move_elephant(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos)
{
    vector<int> possible_move;
    if (elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, -16))
        possible_move.push_back(ucsqPieces[pos] - 16);
    if (elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, 16))
        possible_move.push_back(ucsqPieces[pos] + 16);
    if (elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, -20))
        possible_move.push_back(ucsqPieces[pos] - 20);
    if (elephant_rule(sdPlayer, ucsqPieces, chessboard, pos, 20))
        possible_move.push_back(ucsqPieces[pos] + 20);
    return possible_move;
}
bool horse_rule(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos, int move)
{
    return judgeIn(ucsqPieces[pos] + move) &&
           !(chessboard[ucsqPieces[pos] + move] <= 6 + sdPlayer * 7 &&
             chessboard[ucsqPieces[pos] + move] >= 0 + sdPlayer * 7);
}
vector<int> move_horse(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos)
{
    if (!(chessboard[ucsqPieces[pos] + 1] <= 6 + sdPlayer * 7 &&
          chessboard[ucsqPieces[pos] + 1] >= 0 + sdPlayer * 7) &&
        horse_rule(sdPlayer, ucsqPieces, chessboard, pos, -8))
}

vector<int> move_cart(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos)
{
}

vector<int> move_cannon(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos)
{
}

vector<int> move_soldier(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos)
{
}

vector<int> (*move_chess[7])(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[], int pos) = {};
vector<vector<int>> possibleMove(int sdPlayer, const int (&ucsqPieces)[], const int (&chessboard)[])
{
    vector<vector<int>> possible(32); //所有棋子可能走法先全部置0，这可能会与被吃棋子的置0有冲突，读取方法应与下方循环相同
    for (int i = sdPlayer * 16; i < sdPlayer * 16 + 16; i++)
    {
        if (ucsqPieces[i] != 0)
            possible[i] = move_chess[cnPieceTypes[i]](sdPlayer, ucsqPieces, chessboard, i);
    }
}
int main()
{
    int sdPlayer; // 轮到谁走，0=红方，1=黑方
    int ucsqPieces[32] = {
        86, 85, 87, 84, 88, 81, 88, 80, 90, 71, 65, 63, 61, 59, 57, 55,
        5, 4, 6, 3, 7, 2, 8, 1, 9, 20, 26, 28, 30, 32, 34, 36}; // 每个棋子放的位置，0表示被吃

    vector<int> chessboard(91, 0);
    for (int i = 0; i < 32; i++)
        chessboard[ucsqPieces[i]] = cnPieceTypes[i] + sdPlayer * 7;
    return 0;
}