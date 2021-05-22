#include "Search.h"

#include <algorithm>
#include <ctime>
#include <map>

#include "Evaluate.h"
#include "Position.h"
clock_t startTime = 0;
Eval myeval;
int DEPTH = 5;
int gloTime;
static int bestmove;  // 记录返回的步数
static PositionStruct turn;
static const int turnArr[100] = {
    0,  9,  8,  7,  6,  5,  4,  3,  2,  1,  18, 17, 16, 15, 14, 13, 12, 11, 10,
    27, 26, 25, 24, 23, 22, 21, 20, 19, 36, 35, 34, 33, 32, 31, 30, 29, 28, 45,
    44, 43, 42, 41, 40, 39, 38, 37, 54, 53, 52, 51, 50, 49, 48, 47, 46, 63, 62,
    61, 60, 59, 58, 57, 56, 55, 72, 71, 70, 69, 68, 67, 66, 65, 64, 81, 80, 79,
    78, 77, 76, 75, 74, 73, 90, 89, 88, 87, 86, 85, 84, 83, 82};
static int total = 0;
static int chessType[40] = {0,  1,  1,  2,  2,  3,  3,  4,  4,  5, 5,
                            6,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10,
                            10, 11, 11, 12, 12, 13, 13, 13, 13, 13};
//帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
static map<int, int> subtable[20];
int chessHash(PositionStruct& mychess) {
    int hs = 0;
    for (int i = 0; i < 32; ++i) hs ^= hsh[chessType[i]][mychess.nowPos[i]];
    return hs;
}
struct heuristic {
    int src, des, hh;
    bool operator<(const heuristic& x) { return this->hh > x.hh; }
};
static int hh[100][100];

int alphabeta(PositionStruct& mychess, int depth, int alpha, int beta,
              int nowHsh = 0) {
    if (clock() - startTime >= gloTime) return -100000005;
    int xishu = (mychess.sdPlayer ? -1 : 1);
    int state = mychess.Repeat();
    // cout << mychess.nowPos[0] << " " << mychess.nowPos[16] << endl;
    if (mychess.nowPos[0] == 0 || state == 1)
        return xishu * -6666666;  //红死 红长将
    if (mychess.nowPos[16] == 0 || state == 2)
        return xishu * 6666666;  //黑死 黑长将

    if (state == 0 || depth <= 0) {
        myeval.EvalInit(mychess);
        int tepvalue = myeval.GetEvalNum();
        if (state == 0) {
            if ((mychess.sdPlayer == 1 && tepvalue < -300) ||
                (mychess.sdPlayer == 0 && tepvalue > 300))
                //当前局面走棋方优势较大，但会导致和棋，所以将评估值拉向0
                return 0;
            else
                //当前局面走棋方为劣势，且会导致和棋，所以希望和棋，所以走这一步的意愿加大。
                return xishu * (tepvalue - 1000 * mychess.sdPlayer + 500);
        }
        return tepvalue * xishu;
        // return tepvalue;
    }

    if (nowHsh == 0) nowHsh = chessHash(mychess);
    if (subtable[depth].find(nowHsh) != subtable[depth].end())
        return subtable[depth][nowHsh];

    int chessBoard[100];
    memset(chessBoard, -1, sizeof(chessBoard));
    for (int i = 0; i < 32; ++i) chessBoard[mychess.nowPos[i]] = i;

    vector<vector<int>> moves = mychess.Generate();  //获取走法
    int x = mychess.sdPlayer;

    // 获取所有走法
    vector<heuristic> h;
    for (int i = x * 16; i < x * 16 + 16; ++i)
        for (auto nowPos : moves[i])
            h.push_back(
                {mychess.nowPos[i], nowPos, hh[mychess.nowPos[i]][nowPos]});
    vector<vector<int>>().swap(moves);
    if (h.empty())
        alpha = xishu * 6666666;
    else {
        sort(h.begin(), h.end());
        int testCount = h.size();
        if (DEPTH >= 6 && depth <= DEPTH - 2 && h.size() >= 14 && h[6].hh > 0)
            testCount = 14;

        int nowbestmove = 0;
        for (auto ttem : h) {
            if (!testCount) break;
            testCount--;
            int pastPos = ttem.src;
            if (pastPos == 0) continue;  //已被吃掉则不用管
            int i = chessBoard[pastPos];
            int nowPos = ttem.des;

            int newHsh =
                nowHsh ^ hsh[chessType[i]][pastPos] ^ hsh[chessType[i]][nowPos];
            mychess.sdPlayer = !mychess.sdPlayer;
            int isenemy = chessBoard[nowPos];
            mychess.nowPos[i] = nowPos;
            int tepcount = mychess.Count;
            mychess.Moves.push_back(
                {pastPos, mychess.nowPos[i], isenemy != -1});
            if (isenemy != -1) {
                newHsh ^= hsh[chessType[isenemy]][nowPos];
                mychess.Count = 0;
                mychess.nowPos[isenemy] = 0;  //找到了，要吃掉
            } else {
                ++mychess.Count;
            }

            int score = -alphabeta(mychess, depth - 1, -beta, -alpha, newHsh);
            /*
                for (int i = 0; i < DEPTH - depth; ++i) cout << "     ";
                cout << pastPos << " " << nowPos << " " << score * xishu
                     << endl;
            */

            //复原
            mychess.sdPlayer = !mychess.sdPlayer;
            mychess.nowPos[i] = pastPos;
            mychess.Moves.pop_back();
            if (isenemy != -1) mychess.nowPos[isenemy] = nowPos;
            mychess.Count = tepcount;
            if (score == 100000005) {
                h.clear();
                return -score;
            }
            if (score > alpha) {
                if (depth == DEPTH) bestmove = (pastPos << 8) + nowPos;
                alpha = score;
                nowbestmove = pastPos * 100 + nowPos;
            }

            if (alpha >= beta) break;
        }
        hh[nowbestmove / 100][nowbestmove % 100] += pow(2, depth);  // 历史启发
    }

    h.clear();  // 两种清空内存方法

    subtable[depth][nowHsh] = alpha;  // 历史记录
    for (int i = 0; i < 32; ++i) turn.nowPos[i] = turnArr[mychess.nowPos[i]];
    subtable[depth][chessHash(turn)] = alpha;  // 记录翻转局面

    return alpha;
}

int SearchMain(PositionStruct& mychess, int gotime) {
    startTime = clock();
    memset(hh, 0, sizeof(hh));
    for (int i = 0; i < 20; ++i) subtable[i].clear();
    gloTime = (gotime > 10000 ? (gotime - 1000) : (gotime * 9 / 10));
    int smallbestmove = 0;
    int maxvalue = -100000000;
    for (DEPTH = 2; DEPTH <= 18; DEPTH += 2) {  // 迭代深化
        maxvalue = alphabeta(mychess, DEPTH, -100000000, 100000000);
        if (maxvalue == -100000005) return smallbestmove;
        if (maxvalue == 6666666) {
            cout << "必胜！\n";
            return bestmove;
        }
        if (maxvalue == -6666666) {
            cout << "必输！\n";
            return smallbestmove;
        }

        // if (DEPTH >= 12 && smallbestmove == bestmove) break;
        cout << DEPTH << " " << bestmove << " " << clock() - startTime << endl;
        cout << bestmove / 256 << "→" << bestmove % 256 << endl;
        smallbestmove = bestmove;
        if (clock() - startTime > gotime / 2 && DEPTH == 8) break;
    }
    return smallbestmove;
}
