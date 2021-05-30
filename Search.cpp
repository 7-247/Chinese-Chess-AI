#include "Search.h"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <map>

#include "Evaluate.h"
#include "Position.h"
clock_t startTime = 0;
const int MaxDepth = 20;
Eval myeval;
bool tiaoshismall = 0;
bool tiaoshi = 0;
bool tiaoshibig = 0;
int DEPTH = 3;
int gloTime;
vector<int> nodeNum[MaxDepth + 5];
int totalNode;
struct tree {
    PositionStruct chess;
    int childnode[5];
    // bool childuse[5];  // 1在用 0抛弃
    int childmove[5];
    int fathernode, eval, nowbestnode;
    bool inuse;
} node[100000];

struct allmoves {
    PositionStruct chess;
    int eval, move;
    bool operator<(const allmoves& x) { return this->chess.sdPlayer == 0 ? (this->eval < x.eval) : (this->eval > x.eval); }
    // allmoves(PositionStruct chess, int eval, int move) : chess(chess), eval(eval), move(move){};
};

vector<allmoves> am;

int bestmove;  // 记录返回的步数
int lastbestmove = 0;
static PositionStruct turn;
static const int turnArr[100] = {0,  9,  8,  7,  6,  5,  4,  3,  2,  1,  18, 17, 16, 15, 14, 13, 12, 11, 10, 27, 26, 25, 24, 23, 22, 21, 20, 19, 36, 35, 34,
                                 33, 32, 31, 30, 29, 28, 45, 44, 43, 42, 41, 40, 39, 38, 37, 54, 53, 52, 51, 50, 49, 48, 47, 46, 63, 62, 61, 60, 59, 58, 57,
                                 56, 55, 72, 71, 70, 69, 68, 67, 66, 65, 64, 81, 80, 79, 78, 77, 76, 75, 74, 73, 90, 89, 88, 87, 86, 85, 84, 83, 82};
static int chessType[40] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 13, 13, 13};
//帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
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

int alphabeta(PositionStruct& mychess, int depth, int alpha, int beta, long long nowHsh = 0, int extend = 0) {
    if (clock() - startTime >= gloTime) return 100000005;
    int state = mychess.Repeat();

    if (mychess.nowPos[0] == 0 || state == 1) return -6666666;  //红死 红长将
    if (mychess.nowPos[16] == 0 || state == 2) return 6666666;  //黑死 黑长将

    if (state == 0 || depth + extend <= 0) {
        // ChessBoard outp(mychess);
        // outp.DrawBoard();
        myeval.EvalInit(mychess);
        int tepvalue = myeval.GetEvalNum();
        if (state == 0) {
            cout << "state=000\n";
            cout << tepvalue << endl;
            if ((mychess.sdPlayer == 1 && tepvalue < -300) || (mychess.sdPlayer == 0 && tepvalue > 300)) {
                cout << "拉回0\n";
                //当前局面走棋方优势较大，但会导致和棋，所以将评估值拉向0
                return 0;
            } else
                //当前局面走棋方为劣势，且会导致和棋，所以希望和棋，所以走这一步的意愿加大。
                return tepvalue - 1000 * mychess.sdPlayer + 500;
        }
        if (tiaoshibig) {
            for (int i = DEPTH; i >= depth; --i) cout << "      ";
            cout << tepvalue << endl;
        }
        return tepvalue;
    }

    if (nowHsh == 0) nowHsh = chessHash(mychess);
    if (subtable[depth + 4].find(nowHsh) != subtable[depth + 4].end()) return subtable[depth + 4][nowHsh];

    int chessBoard[100];
    memset(chessBoard, -1, sizeof(chessBoard));
    for (int i = 0; i < 32; ++i) chessBoard[mychess.nowPos[i]] = i;

    vector<vector<int>> moves = mychess.Generate();  //获取走法
    int x = mychess.sdPlayer;

    // 获取所有走法
    vector<heuristic> h;
    for (int i = x * 16; i < x * 16 + 16; ++i)
        for (auto nowPos : moves[i]) {
            if (mychess.nowPos[i] == 0 || nowPos == 0) continue;
            if (nowPos == mychess.nowPos[0]) {
                if (DEPTH == depth) am.push_back({mychess, -6666667, mychess.nowPos[i] * 100 + nowPos});
                return -6666666;
            } else if (nowPos == mychess.nowPos[16]) {
                if (DEPTH == depth) am.push_back({mychess, 6666667, mychess.nowPos[i] * 100 + nowPos});
                return 6666666;
            }
            int typ = chessType[chessBoard[mychess.nowPos[i]]];
            int ttv = hh[mychess.nowPos[i]][nowPos];
            if ((typ == 3 || typ == 4 || typ == 10 || typ == 11) && ttv == 0) ttv = 1;
            if (DEPTH == depth && ((mychess.nowPos[i] << 8) + nowPos == lastbestmove)) ttv += 100000000;
            /*
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
            */
            h.push_back({mychess.nowPos[i], nowPos, ttv});
        }
    vector<vector<int>>().swap(moves);
    int best = -123456789;
    if (h.empty())
        best = mychess.sdPlayer == 1 ? 6666666 : -6666666;
    else {
        sort(h.begin(), h.end());
        int testCount = 1000;
        /*
        int testCount = min(int(h.size()), index[DEPTH - depth]);
        if (testCount >= 16 && h[testCount / 2].hh)
            while (h[testCount].hh == 0) testCount--;
        */

        int nowbestmove = 0;
        bool isfirst = 1;
        bool bre = 0;
        for (auto ttem : h) {
            if (tiaoshibig) {
                for (int i = DEPTH; i >= depth; --i) cout << "      ";
                cout << ttem.src << " " << ttem.des << " " << mychess.sdPlayer << endl;
            }

            if (!testCount) break;
            testCount--;
            int pastPos = ttem.src;
            if (pastPos == 0) continue;  //已被吃掉则不用管
            int i = chessBoard[pastPos];
            int nowPos = ttem.des;
            long long newHsh = nowHsh ^ hsh[chessType[i]][pastPos] ^ hsh[chessType[i]][nowPos];
            mychess.sdPlayer = !mychess.sdPlayer;
            int isenemy = chessBoard[nowPos];
            mychess.nowPos[i] = nowPos;
            int tepcount = mychess.Count;
            mychess.Moves.push_back({pastPos, mychess.nowPos[i], isenemy != -1});
            if (isenemy != -1) {
                newHsh ^= hsh[chessType[isenemy]][nowPos];
                mychess.Count = 0;
                mychess.nowPos[isenemy] = 0;  //找到了，要吃掉
            } else
                ++mychess.Count;
            int value;
            if (isfirst) {
                isfirst = 0;
                // value = alphabeta(mychess, depth - 1, alpha, beta, newHsh, isenemy == -1 ? 0 : 4);
                value = alphabeta(mychess, depth - 1, alpha, beta, newHsh, isenemy == -1 ? 0 : 1);
                if (tiaoshibig) {
                    for (int i = DEPTH; i >= depth; --i) cout << "      ";
                    cout << alpha << " " << value << " " << beta << endl;
                }
                if (depth == DEPTH) bestmove = (pastPos << 8) + nowPos;
                nowbestmove = pastPos * 100 + nowPos;
            } else {
                if (mychess.sdPlayer == 1) {
                    // value = alphabeta(mychess, depth - 1, alpha, alpha + 1, newHsh, isenemy == -1 ? 0 : 4);
                    value = alphabeta(mychess, depth - 1, alpha, alpha + 1, newHsh, isenemy == -1 ? 0 : 1);
                    if (tiaoshibig) {
                        for (int i = DEPTH; i >= depth; --i) cout << "      ";
                        cout << alpha << " " << value << " " << alpha + 1 << endl;
                    }
                    if (alpha < value && value < beta) {
                        // value = alphabeta(mychess, depth - 1, value, beta, newHsh, isenemy == -1 ? 0 : 4);
                        value = alphabeta(mychess, depth - 1, value, beta, newHsh, isenemy == -1 ? 0 : 1);
                        if (tiaoshibig) {
                            for (int i = DEPTH; i >= depth; --i) cout << "      ";
                            cout << " / " << value << " " << beta << endl;
                        }
                    }

                } else {
                    // value = alphabeta(mychess, depth - 1, beta - 1, beta, newHsh, isenemy == -1 ? 0 : 4);
                    value = alphabeta(mychess, depth - 1, beta - 1, beta, newHsh, isenemy == -1 ? 0 : 1);
                    if (tiaoshibig) {
                        for (int i = DEPTH; i >= depth; --i) cout << "      ";
                        cout << beta - 1 << " " << value << " " << beta << endl;
                    }
                    if (alpha < value && value < beta) {
                        // value = alphabeta(mychess, depth - 1, alpha, value, newHsh, isenemy == -1 ? 0 : 4);
                        value = alphabeta(mychess, depth - 1, alpha, value, newHsh, isenemy == -1 ? 0 : 1);
                        if (tiaoshibig) {
                            for (int i = DEPTH; i >= depth; --i) cout << "      ";
                            cout << alpha << " " << value << " / " << endl;
                        }
                    }
                }
            }
            if (value == 100000005) {
                bre = 1;
                goto br;
            }
            if (mychess.sdPlayer == 1) {  // Max 面向的是下一个结点
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
            if (DEPTH == depth) {
                am.push_back({mychess, value, pastPos * 100 + nowPos});
            }
        br:
            //复原
            mychess.sdPlayer = !mychess.sdPlayer;
            mychess.nowPos[i] = pastPos;
            mychess.Moves.pop_back();
            if (isenemy != -1) mychess.nowPos[isenemy] = nowPos;
            mychess.Count = tepcount;

            if (bre) {
                h.clear();
                return 100000005;
            }
            if (alpha > beta) break;
        }
        hh[nowbestmove / 100][nowbestmove % 100] += pow(2, depth);  // 历史启发
    }

    h.clear();  // 两种清空内存方法
    if (best != -123456789 && beta - alpha > 1) {
        subtable[depth + 4][nowHsh] = best;  // 历史记录
        for (int i = 0; i < 32; ++i) turn.nowPos[i] = turnArr[mychess.nowPos[i]];
        subtable[depth + 4][chessHash(turn)] = best;  // 记录翻转局面
        return best;
    }

    return mychess.sdPlayer ? beta : alpha;
}

int oldSearchMain(PositionStruct& mychess, int nownode, int nowDepth) {
    memset(hh, 0, sizeof(hh));
    am.clear();
    for (int i = 0; i < MaxDepth; ++i) subtable[i].clear();
    int nowvalue = -100000000;
    DEPTH = 3;
    nowvalue = alphabeta(mychess, DEPTH, -100000000, 100000000);
    if (nowvalue == 100000005) return nowvalue;
    node[nownode].eval = nowvalue;
    sort(am.begin(), am.end());
    node[nownode].nowbestnode = 0;
    int cnt = 0;
    for (auto i : am) {
        ++totalNode;
        node[nownode].childnode[cnt] = totalNode;
        // node[nownode].childuse[cnt] = 1;
        node[nownode].childmove[cnt] = i.move;
        nodeNum[nowDepth + 1].push_back(totalNode);
        node[totalNode].fathernode = nownode;
        node[totalNode].chess = i.chess;
        node[totalNode].eval = i.eval;
        node[totalNode].inuse = 1;
        ++cnt;
        if (cnt == 5) break;
    }

    return nowvalue;
}
void discard(int x) {
    if (x == 0) return;
    node[x].inuse = 0;
    for (int i = 0; i < 5; ++i)
        if (node[node[x].childnode[i]].inuse) discard(node[x].childnode[i]);
}
int SearchMain(PositionStruct& mychess, int gotime) {
    startTime = clock();
    int lastTime = startTime;
    gloTime = max(gotime - 1000, gotime * 9 / 10);
    totalNode = 1;
    for (int i = 0; i < MaxDepth + 5; ++i) nodeNum[i].clear();
    nodeNum[1].push_back(1);
    memset(node, 0, sizeof(node));
    node[1].chess = mychess;
    node[1].inuse = 1;

    // for (int nowDepth = 1; nowDepth <= 1; nowDepth += 1) {
    for (int nowDepth = 1; nowDepth <= MaxDepth - 5; nowDepth += 1) {
        for (auto j : nodeNum[nowDepth])
            if (node[j].inuse) {
                int nowDepthEval = oldSearchMain(node[j].chess, j, nowDepth);
                if (nowDepthEval == 100000005) goto op;
                node[j].eval = nowDepthEval;
                if (tiaoshi) {
                    for (int k = 1; k < nowDepth; ++k) cout << "   ";
                    cout << j << "/" << totalNode << "  " << clock() - startTime << endl;
                }

                // ChessBoard outp(node[j].chess);
                // outp.DrawBoard();
                /*
                for (int i = 0; i < 5; ++i) {
                    if (node[node[j].childnode[i]].inuse) {
                        for (int k = 1; k < nowDepth; ++k) cout << "   ";
                        int tm = node[j].childmove[i];
                        printf("%02d->%02d eval=", tm / 100, tm % 100);
                        cout << node[node[j].childnode[i]].eval << endl;
                    }
                }
                */
            }
        for (auto j : nodeNum[nowDepth - 1]) node[j].chess.Moves.clear();
        for (int i = nowDepth - 1; i; --i) {
            int player = node[nodeNum[i][0]].chess.sdPlayer;
            for (auto j : nodeNum[i]) node[j].eval = (player == 1 ? 100000000 : -100000000);
            int fa = 0, facount = 0;
            for (auto j : nodeNum[i + 1]) {
                if (node[j].fathernode != fa)
                    facount = 0;
                else
                    facount++;
                fa = node[j].fathernode;
                if (player == 1) {
                    if (node[j].eval < node[fa].eval) {
                        node[fa].eval = node[j].eval;
                        node[fa].nowbestnode = facount;
                    }

                } else {
                    if (node[j].eval > node[fa].eval) {
                        node[fa].eval = node[j].eval;
                        node[fa].nowbestnode = facount;
                    }
                }
            }
            for (auto j : nodeNum[i + 1])
                if (abs(node[j].eval - node[node[j].fathernode].eval) > 50) discard(j);
        }
        if (tiaoshi) {
            for (int i = 1; i <= nowDepth; ++i) {
                for (auto j : nodeNum[i])
                    if (node[j].inuse) {
                        for (int k = 1; k < i; ++k) cout << "      ";
                        printf("%4d   %4d        ", node[j].eval, node[j].nowbestnode);
                        for (int k = 0; k < 5; ++k)
                            if (node[node[j].childnode[k]].inuse) printf("%4d ", node[node[j].childnode[k]].eval);

                        cout << endl;
                    }
            }
        }
        if (tiaoshismall) {
            printf("Depth=%-2d timeUse=%-5d totalTimeUse=%-5d\n", nowDepth, clock() - lastTime, clock() - startTime);
            for (int i = 0; i < 5; ++i)
                if (node[node[1].childnode[i]].inuse) printf("%04d  %d\n", node[1].childmove[i], node[node[1].childnode[i]].eval);
        }

        if (clock() - lastTime > gotime / 2) goto op;
        lastTime = clock();
        if (abs(node[1].eval) > 6000000) {
            for (int k = 0; k < 5; ++k)
                if (node[node[1].childnode[k]].inuse && (node[1].childmove[k] % 100 == mychess.nowPos[0] || node[1].childmove[k] % 100 == mychess.nowPos[16])) {
                    node[1].nowbestnode = k;
                    break;
                }
            break;
        }

        int zoufa = 0;
        for (int i = 0; i < 5; ++i)
            if (node[node[1].childnode[i]].inuse) ++zoufa;
        if (zoufa <= 1) break;
    }
op:
    bestmove = node[1].childmove[node[1].nowbestnode];
    return ((bestmove / 100) << 8) + (bestmove % 100);
}