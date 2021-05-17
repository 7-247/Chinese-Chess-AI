#include "Evaluate.h"
#include "Position.h"
const int DEPTH = 5;
const int EXP = 0;
// alpha - beta >= EXP时剪枝，EXP为0时保证对DEPTH层的最优性(该值越小剪枝越粗略)
static int bestmove;  //记录返回的步数
/*static const int PosHash [32] =
                {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                22, 23, 24, 25, 26, 27, 28, 29, 30, 31};//按顺序

//优先考虑車，然后是炮、马、兵、象、士、帅*/
static const int PosHash[32] = {7,  8,  9,  10, 5,  6,  12, 14, 3,  4,  1,
                                2,  11, 15, 13, 0,  23, 24, 25, 26, 21, 22,
                                28, 30, 19, 20, 17, 18, 27, 31, 29, 16};
int alphabeta(PositionStruct& mychess, int depth, int alpha, int beta) {
    int state = -1 /*mychess.Repeat()*/;
    if (depth == 0 || mychess.ucsqPieces[0] == 0 ||
        mychess.ucsqPieces[16] == 0 || state != -1) {
        if (mychess.ucsqPieces[0] == 0 || state == 1)  //帅 死了
            return -6666666;
        else if (mychess.ucsqPieces[16] == 0 || state == 2)  //将 死了
            return 6666666;
        else {  // else if (state == 0 || depth == 0)
            Eval myeval(mychess);
            int tepvalue = myeval.GetEvalNum();
            if (state == 0) {
                if ((mychess.sdPlayer == 0 && tepvalue < 0) ||
                    (mychess.sdPlayer == 1 && tepvalue > 0)) {
                    //当前是红方且此时红方优势不大；或者当前是黑方且此时黑方优势不大
                    return tepvalue + 200;  //调高评估值，你愿意和我便和你和
                }
                if ((mychess.sdPlayer == 1 && tepvalue < 0) ||
                    (mychess.sdPlayer == 0 && tepvalue > 0)) {
                    //当前是红方且此时红方有优势；或者当前是黑方且此时黑方有优势
                    return tepvalue - 200;  //调低评估值，你愿意和我偏不和你和
                }
            } else
                return tepvalue;
        }
    } else if (mychess.sdPlayer == 0) {                //红方为极大节点
        vector<vector<int>> tep = mychess.Generate();  //获取走法
        for (int k = 0; k < 16; ++k) {
            int i = PosHash[k];
            int pastpos = mychess.ucsqPieces[i];
            if (pastpos == 0)  //已被吃掉则不用管
                continue;
            for (int j = 0; j < tep[i].size(); ++j) {
                //走一步
                mychess.sdPlayer = !mychess.sdPlayer;
                int isenemy =
                    PosToNo(tep[i][j], mychess.ucsqPieces);  //从位置找序号
                mychess.ucsqPieces[i] = tep[i][j];           //走一步

                if (isenemy != -1) {
                    mychess.Moves.push_back(
                        {pastpos, mychess.ucsqPieces[i], 1});
                    mychess.Count = 0;
                    mychess.ucsqPieces[isenemy] = 0;  //找到了，要吃掉
                } else {
                    mychess.Moves.push_back(
                        {pastpos, mychess.ucsqPieces[i], 0});  //没吃子
                    ++mychess.Count;
                }

                int tepalpha = alphabeta(mychess, depth - 1, alpha, beta);
                if (tepalpha > alpha) {
                    alpha = tepalpha;
                    if (depth == DEPTH) bestmove = (pastpos << 8) + tep[i][j];
                }

                //复原
                mychess.sdPlayer = !mychess.sdPlayer;
                mychess.ucsqPieces[i] = pastpos;
                if (isenemy != -1) mychess.ucsqPieces[isenemy] = tep[i][j];
                if (alpha - beta >= EXP) break;
            }
            if (alpha - beta >= EXP) break;  //外层循环也要跳
        }
        vector<vector<int>>().swap(tep);  //清空内存
        return alpha;
    } else {                                           //黑方为MIN节点
        vector<vector<int>> tep = mychess.Generate();  //输出走法
        for (int k = 16; k < 32; ++k) {
            int i = PosHash[k];
            int pastpos = mychess.ucsqPieces[i];
            if (pastpos == 0)  //被吃掉了不用管
                continue;
            for (int j = 0; j < tep[i].size(); ++j) {  //走法只有前16个是有值的
                mychess.sdPlayer = !mychess.sdPlayer;
                int isenemy =
                    PosToNo(tep[i][j], mychess.ucsqPieces);  //从位置找序号
                mychess.ucsqPieces[i] = tep[i][j];           //走了一步

                if (isenemy != -1) {
                    mychess.Moves.push_back(
                        {pastpos, mychess.ucsqPieces[i], 1});
                    mychess.Count = 0;
                    mychess.ucsqPieces[isenemy] = 0;  //找到了，要吃掉
                } else {
                    mychess.Moves.push_back(
                        {pastpos, mychess.ucsqPieces[i], 0});
                    ++mychess.Count;
                }
                int tepbeta = alphabeta(mychess, depth - 1, alpha, beta);
                if (tepbeta < beta) {
                    beta = tepbeta;
                    if (depth == DEPTH) bestmove = (pastpos << 8) + tep[i][j];
                }

                //复原
                mychess.sdPlayer = !mychess.sdPlayer;
                mychess.ucsqPieces[i] = pastpos;
                if (isenemy != -1) mychess.ucsqPieces[isenemy] = tep[i][j];
                if (alpha - beta >= EXP) break;
            }
            if (alpha - beta >= EXP) break;  //外层循环也要跳
        }
        vector<vector<int>>().swap(tep);  //清空内存
        return beta;
    }
    return -55555555;  //防warning 实际上应该不会返回会这个值
}
int SearchMain(PositionStruct& mychess, int gotime) {
    bestmove = 0;
    int maxvalue =
        alphabeta(mychess, DEPTH, -100000000, 100000000);  //正亿-负亿
    cout << "max score:" << maxvalue << endl;
    return bestmove;
}
