#include "Evaluate.h"
const int DEPTH = 4;
static int bestmove;
int alphabeta(PositionStruct& mychess, int depth, int alpha, int beta) {
    if (depth == 0 || mychess.ucsqPieces[0] == 0 ||
        mychess.ucsqPieces[16] == 0) {
        Eval myeval(mychess);
        return myeval.GetEvalNum();
    } else if (mychess.sdPlayer == 0) {                //红方为极大节点
        vector<vector<int>> tep = mychess.Generate();  //输出走法
        for (int i = 0; i < 16; ++i) {
            int pastpos = mychess.ucsqPieces[i];
            if (pastpos == 0)  //被吃掉了
                continue;
            for (int j = 0; j < tep[i].size(); ++j) {
                //走一步
                mychess.sdPlayer = !mychess.sdPlayer;
                mychess.ucsqPieces[i] = tep[i][j];

                int isenemy =
                    PosToNo(tep[i][j], mychess.ucsqPieces);  //从位置找序号
                if (isenemy != -1)
                    mychess.ucsqPieces[isenemy] = 0;  //找到了，要吃掉

                // alpha = max(alpha,alphabeta(mychess, depth - 1, alpha,beta));
                int tepalpha = alphabeta(mychess, depth - 1, alpha, beta);
                if (tepalpha > alpha) {
                    alpha = tepalpha;
                    if (depth == DEPTH) bestmove = (pastpos << 8) + tep[i][j];
                }

                //复原
                mychess.sdPlayer = !mychess.sdPlayer;
                mychess.ucsqPieces[i] = pastpos;
                if (isenemy != -1) mychess.ucsqPieces[isenemy] = tep[i][j];
                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;  //外层循环也要跳
        }
        return alpha;
    } else {                                           //黑方为MIN节点
        vector<vector<int>> tep = mychess.Generate();  //输出走法

        for (int i = 16; i < 32; ++i) {
            int pastpos = mychess.ucsqPieces[i];
            if (pastpos == 0)  //被吃掉了
                continue;
            for (int j = 0; j < tep[i].size(); ++j) {  //走法只有前16个是有值的
                mychess.sdPlayer = !mychess.sdPlayer;
                mychess.ucsqPieces[i] = tep[i][j];  //走了一步

                int isenemy =
                    PosToNo(tep[i][j], mychess.ucsqPieces);  //从位置找序号
                if (isenemy != -1)
                    mychess.ucsqPieces[isenemy] = 0;  //找到了，要吃掉

                // beta = min(beta, alphabeta(mychess, depth - 1, alpha, beta));
                int tepbeta = alphabeta(mychess, depth - 1, alpha, beta);
                if (tepbeta < beta) {
                    beta = tepbeta;
                    if (depth == DEPTH) bestmove = (pastpos << 8) + tep[i][j];
                }

                //复原
                mychess.sdPlayer = !mychess.sdPlayer;
                mychess.ucsqPieces[i] = pastpos;
                if (isenemy != -1) mychess.ucsqPieces[isenemy] = tep[i][j];

                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;  //外层循环也要跳
        }
        return beta;
    }
}
int SearchMain(PositionStruct& mychess, int gotime) {
    bestmove = 0;
    int maxvalue = alphabeta(mychess, DEPTH, INT_MIN, INT_MAX);
    cout << "max score:" << maxvalue << endl;
    return bestmove;
}
