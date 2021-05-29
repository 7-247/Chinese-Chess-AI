#include "Search.h"

#include <algorithm>
#include <ctime>
#include <map>

#include "Evaluate.h"
#include "Position.h"
#include "book.h"
BookStruct mybook;
clock_t startTime = 0;
const int MaxDepth = 20;
Eval myeval;
int DEPTH = 5;
int gloTime;
int index[] = {50, 45, 40, 35, 30, 25, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 10, 10, 10, 10};
int bestmove;  // ��¼���صĲ���
int lastbestmove = 0;
static PositionStruct turn;
static const int turnArr[100] = {
    0,  9,  8,  7,  6,  5,  4,  3,  2,  1,  18, 17, 16, 15, 14, 13, 12, 11, 10,
    27, 26, 25, 24, 23, 22, 21, 20, 19, 36, 35, 34, 33, 32, 31, 30, 29, 28, 45,
    44, 43, 42, 41, 40, 39, 38, 37, 54, 53, 52, 51, 50, 49, 48, 47, 46, 63, 62,
    61, 60, 59, 58, 57, 56, 55, 72, 71, 70, 69, 68, 67, 66, 65, 64, 81, 80, 79,
    78, 77, 76, 75, 74, 73, 90, 89, 88, 87, 86, 85, 84, 83, 82};
static int chessType[40] = {0,  1,  1,  2,  2,  3,  3,  4,  4,  5, 5,
                            6,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10,
                            10, 11, 11, 12, 12, 13, 13, 13, 13, 13};
//˧�����������������ڱ���������(��ʿʿ������������������������)
static map<long long, int> subtable[MaxDepth];
long long chessHash(PositionStruct& mychess) {
    long long hs = 0;
    for (int i = 0; i < 32; ++i) hs ^= hsh[chessType[i]][mychess.nowPos[i]];
    return hs;
}
struct heuristic {
    int src, des, hh;
    bool operator<(const heuristic& x) { return this->hh > x.hh; }
};
static int hh[100][100];

int alphabeta(PositionStruct& mychess, int depth, int alpha, int beta,
              long long nowHsh = 0, int extend = 0) {
    if (clock() - startTime >= gloTime) return 100000005;
    int state = mychess.Repeat();
    if (mychess.nowPos[0] == 0 || state == 1) return -6666666;  //���� �쳤��
    if (mychess.nowPos[16] == 0 || state == 2) return 6666666;  //���� �ڳ���

    if (state == 0 || depth + extend <= 0) {
        myeval.EvalInit(mychess);
        int tepvalue = myeval.GetEvalNum();
        if (state == 0) {
            if ((mychess.sdPlayer == 1 && tepvalue < -300) ||
                (mychess.sdPlayer == 0 && tepvalue > 300))
                //��ǰ�������巽���ƽϴ󣬵��ᵼ�º��壬���Խ�����ֵ����0
                return 0;
            else
                //��ǰ�������巽Ϊ���ƣ��һᵼ�º��壬����ϣ�����壬��������һ������Ը�Ӵ�
                return tepvalue - 1000 * mychess.sdPlayer + 500;
        }
        return tepvalue;
    }

    if (nowHsh == 0) nowHsh = chessHash(mychess);
    if (subtable[depth + 4].find(nowHsh) != subtable[depth + 4].end())
        return subtable[depth + 4][nowHsh];

    int chessBoard[100];
    memset(chessBoard, -1, sizeof(chessBoard));
    for (int i = 0; i < 32; ++i) chessBoard[mychess.nowPos[i]] = i;

    vector<vector<int>> moves = mychess.Generate();  //��ȡ�߷�
    int x = mychess.sdPlayer;

    // ��ȡ�����߷�
    vector<heuristic> h;
    for (int i = x * 16; i < x * 16 + 16; ++i)
        for (auto nowPos : moves[i]) {
            if (mychess.nowPos[i] == 0 || nowPos == 0) continue;
            int typ = chessType[chessBoard[mychess.nowPos[i]]];
            int ttv = hh[mychess.nowPos[i]][nowPos];
            if ((typ == 3 || typ == 4 || typ == 10 || typ == 11) && ttv == 0)
                ttv = 1;
            if (DEPTH == depth &&
                ((mychess.nowPos[i] << 8) + nowPos == lastbestmove))
                ttv += 100000000;

            if (mychess.sdPlayer == 1) {
                if (mychess.sdPlayer) mychess.sdPlayer = !mychess.sdPlayer;
                int pastPos = mychess.nowPos[i];
                mychess.nowPos[i] = nowPos;
                int isenemy = chessBoard[nowPos];
                if (isenemy != -1) mychess.nowPos[isenemy] = 0;

                myeval.EvalInit(mychess);
                if (mychess.sdPlayer == 1)
                    ttv += myeval.GetEvalNum();
                else
                    ttv -= myeval.GetEvalNum();

                mychess.sdPlayer = !mychess.sdPlayer;
                mychess.nowPos[i] = pastPos;
                if (isenemy != -1) mychess.nowPos[isenemy] = nowPos;
            }

            h.push_back({mychess.nowPos[i], nowPos, ttv});
        }
    vector<vector<int>>().swap(moves);
    int best = -123456789;
    if (h.empty())
        best = mychess.sdPlayer == 1 ? 6666666 : -6666666;
    else {
        sort(h.begin(), h.end());

        int testCount = min(int(h.size()), index[DEPTH - depth]);
        if (testCount >= 16 && h[testCount / 2].hh)
            while (h[testCount].hh == 0) testCount--;

        int nowbestmove = 0;
        bool isfirst = 1;
        bool bre = 0;
        for (auto ttem : h) {
            if (!testCount) break;
            testCount--;
            int pastPos = ttem.src;
            if (pastPos == 0) continue;  //�ѱ��Ե����ù�
            int i = chessBoard[pastPos];
            int nowPos = ttem.des;
            long long newHsh =
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
                mychess.nowPos[isenemy] = 0;  //�ҵ��ˣ�Ҫ�Ե�
            } else
                ++mychess.Count;
            int value;
            if (isfirst) {
                isfirst = 0;
                value = alphabeta(mychess, depth - 1, alpha, beta, newHsh,
                                  isenemy == -1 ? 0 : 4);
                if (depth == DEPTH) bestmove = (pastPos << 8) + nowPos;
                nowbestmove = pastPos * 100 + nowPos;
            } else {
                if (mychess.sdPlayer == 1) {
                    value = alphabeta(mychess, depth - 1, alpha, alpha + 1,
                                      newHsh, isenemy == -1 ? 0 : 4);
                    if (alpha < value && value < beta) {
                        value = alphabeta(mychess, depth - 1, value, beta,
                                          newHsh, isenemy == -1 ? 0 : 4);
                    }

                } else {
                    value = alphabeta(mychess, depth - 1, beta - 1, beta,
                                      newHsh, isenemy == -1 ? 0 : 4);
                    if (alpha < value && value < beta) {
                        value = alphabeta(mychess, depth - 1, alpha, value,
                                          newHsh, isenemy == -1 ? 0 : 4);
                    }
                }
            }
            if (value == 100000005) {
                bre = 1;
                goto br;
            }
            if (mychess.sdPlayer == 1) {  // Max ���������һ�����
                if (value > alpha) {
                    best = alpha = value;
                    if (depth == DEPTH) bestmove = (pastPos << 8) + nowPos;
                    nowbestmove = pastPos * 100 + nowPos;
                }
            } else {
                if (value < beta) {
                    best = beta = value;
                    if (depth == DEPTH) bestmove = (pastPos << 8) + nowPos;
                    nowbestmove = pastPos * 100 + nowPos;
                }
            }

            if (alpha >= beta) goto br;

        br:
            //��ԭ
            mychess.sdPlayer = !mychess.sdPlayer;
            mychess.nowPos[i] = pastPos;
            mychess.Moves.pop_back();
            if (isenemy != -1) mychess.nowPos[isenemy] = nowPos;
            mychess.Count = tepcount;

            if (bre) {
                h.clear();
                return 100000005;
            }
        }
        hh[nowbestmove / 100][nowbestmove % 100] += pow(2, depth);  // ��ʷ����
    }

    h.clear();  // ��������ڴ淽��
    if (best != -123456789 && beta - alpha > 1) {
        subtable[depth + 4][nowHsh] = best;  // ��ʷ��¼
        for (int i = 0; i < 32; ++i)
            turn.nowPos[i] = turnArr[mychess.nowPos[i]];
        subtable[depth + 4][chessHash(turn)] = best;  // ��¼��ת����
        return best;
    }

    return mychess.sdPlayer ? beta : alpha;
}

int SearchMain(PositionStruct& mychess, int gotime) {
    int mymove = mybook.find(mychess);
    if (mymove == 0) {
        startTime = clock();
        int lastTime = startTime;
        memset(hh, 0, sizeof(hh));
        for (int i = 0; i < MaxDepth; ++i) subtable[i].clear();
        gloTime = (gotime > 10000 ? (gotime - 1000) : (gotime * 9 / 10));

        int maxvalue = -100000000;
        // for (DEPTH = 2; DEPTH <= MaxDepth - 5; DEPTH += 1) {  // �����
        for (DEPTH = 2; DEPTH <= MaxDepth - 5; DEPTH += 1) {  // �����
            maxvalue = alphabeta(mychess, DEPTH, -100000000, 100000000);
            if (maxvalue == 100000005) return lastbestmove;
            if (maxvalue == 6666666) {
                //printf("�췽��ʤ��\nDepth = %d\n", DEPTH);
                return bestmove;
            }
            if (maxvalue == -6666666) {
                //printf("�ڷ���ʤ��\nDepth = %d\n", DEPTH);
                return lastbestmove ? lastbestmove : bestmove;
            }
            //printf("%2d  %2d��%-2d  �����ֵ%-7d  ��ʱ%5dms  �ܺ�ʱ%5dms\n",
             //      DEPTH, bestmove / 256, bestmove % 256, maxvalue,
             //      int(clock() - lastTime), int(clock() - startTime));
            lastbestmove = bestmove;
            if (clock() - startTime > gotime / 4 && DEPTH == 8) break;
            lastTime = clock();
        }
        return lastbestmove;
    } else
        return mymove;
}
