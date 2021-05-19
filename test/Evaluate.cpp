#include "Evaluate.h"

inline int mirrorFlip(int position) {  //对于黑方，请镜像翻转
    return 91 - position;
}
inline int calculateBlackEval(int position, int type, int gonum) {
    return (StartPositionValue[type][mirrorFlip(position)] + initvalue[type] +
            govalue[type] * gonum);
}
inline int calculateRedEval(int position, int type, int gonum) {
    return (StartPositionValue[type][position] + initvalue[type] +
            govalue[type] * gonum);
}
Eval::Eval(PositionStruct& pos) {
    EvalRed = EvalBlack = 0;
    int ThreatenValue[32] = {0};  //敌方的威胁
    int ProtectValue[32] = {0};   //己方的保护
    int gonum[32] = {0};          //机动性
    vector<vector<int>> tep[2];

    tep[pos.sdPlayer] = pos.Predict();  // 0为红，1为黑
    pos.sdPlayer = !pos.sdPlayer;
    tep[pos.sdPlayer] = pos.Predict();
    pos.sdPlayer = !pos.sdPlayer;  //换回来

    for (int i = 0; i < 16; ++i) {  //考虑红方
        if (pos.ucsqPieces[i]) {
            for (size_t j = 0; j < tep[0][i].size(); ++j) {
                int pieceNo = PosToNo(tep[0][i][j], pos.ucsqPieces);
                if (pieceNo == -1)
                    ++gonum[i];
                else if (pieceNo >= 16) {      //黑
                    ++ThreatenValue[pieceNo];  //威胁系数
                    ++gonum[i];
                } else {  // 无需额外判断了，必定是友方
                          // if(isfriend(pieceNo,po.sdPlayer))
                    ++ProtectValue[pieceNo];  //保护系数
                }
            }
        }
    }
    for (int i = 16; i < 32; ++i) {  //黑
        if (pos.ucsqPieces[i]) {
            for (size_t j = 0; j < tep[1][i].size(); ++j) {
                int pieceNo = PosToNo(tep[1][i][j], pos.ucsqPieces);
                if (pieceNo == -1)
                    ++gonum[i];
                else if (pieceNo < 16) {       //红
                    ++ThreatenValue[pieceNo];  //威胁系数
                    ++gonum[i];
                } else {  // 无需额外判断了，必定是友方
                          // if(isfriend(pieceNo,po.sdPlayer))
                    ++ProtectValue[pieceNo];  //保护系数
                }
            }
        }
    }

    if (pos.sdPlayer == 0) {  //假如现在是红方准备走棋
        int MaxthreatenNo = -1, MaxthreatenValue = -999999;
        for (int i = 0; i < 16; ++i) {
            if (pos.ucsqPieces[i]) {
                int value = calculateRedEval(pos.ucsqPieces[i], cnPieceTypes[i],
                                             gonum[i]);
                if (ThreatenValue[i] >
                    ((i % 16 == 0) ? 0 : ProtectValue[i])) {  //保护不过来了
                    if (value > MaxthreatenValue) {
                        if (MaxthreatenNo != -1)          //已经发现过了
                            EvalRed -= MaxthreatenValue;  //补扣
                        MaxthreatenNo = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue = value;
                    }
                }
                EvalRed += value;
                //因为此时我方是先手，故不必扣除此处分数，后面补扣
            }
        }
        MaxthreatenNo = -1, MaxthreatenValue = -999999;
        for (int i = 16; i < 32; ++i) {
            if (pos.ucsqPieces[i]) {
                int value = calculateBlackEval(pos.ucsqPieces[i],
                                               cnPieceTypes[i], gonum[i]);
                if (ThreatenValue[i] >
                    ((i % 16 == 0) ? 0 : ProtectValue[i])) {  //保护不过来了
                    if (value > MaxthreatenValue) {
                        if (MaxthreatenNo != -1)            //已经发现过了
                            EvalBlack += MaxthreatenValue;  //补加
                        MaxthreatenNo = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue = value;
                    }
                } else  //此处不加，后面补加
                    EvalBlack += value;
            }
        }
    } else {  //假如现在是黑方准备走棋
        int MaxthreatenNo = -1, MaxthreatenValue = -999999;
        for (int i = 16; i < 32; ++i) {
            if (pos.ucsqPieces[i]) {
                int value = calculateBlackEval(pos.ucsqPieces[i],
                                               cnPieceTypes[i], gonum[i]);
                if (ThreatenValue[i] >
                    ((i % 16 == 0) ? 0 : ProtectValue[i])) {  //保护不过来了
                    if (value > MaxthreatenValue) {
                        if (MaxthreatenNo != -1)            //已经发现过了
                            EvalBlack -= MaxthreatenValue;  //补扣
                        MaxthreatenNo = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue = value;
                    }
                }
                EvalBlack += value;
                //因为此时我方是先手，故不必扣除此处分数，后面补扣
            }
        }
        MaxthreatenNo = -1, MaxthreatenValue = -999999;
        for (int i = 0; i < 16; ++i) {
            if (pos.ucsqPieces[i]) {
                int value = calculateRedEval(pos.ucsqPieces[i], cnPieceTypes[i],
                                             gonum[i]);
                if (ThreatenValue[i] >
                    ((i % 16 == 0) ? 0 : ProtectValue[i])) {  //保护不过来了
                    if (value > MaxthreatenValue) {
                        if (MaxthreatenNo != -1)          //已经发现过了
                            EvalRed += MaxthreatenValue;  //补加
                        MaxthreatenNo = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue = value;
                    }
                } else  //此处不加，后面补加
                    EvalRed += value;
            }
        }
    }

    vector<vector<int>>().swap(tep[0]);
    vector<vector<int>>().swap(tep[1]);
}
