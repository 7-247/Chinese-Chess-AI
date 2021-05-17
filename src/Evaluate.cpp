#include "Evaluate.h"

inline int mirrorFlip(int position) {  //对于黑方，请镜像翻转
    return 91 - position;
}
inline void calculateEval(int position, int type, int& evalred, int& evalblack,
                          int gonum,
                          bool sdPlayer) {  //固定价值、机动价值、位置价值
    if (sdPlayer)
        evalblack += (StartPositionValue[type][mirrorFlip(position)] +
                      initvalue[type] + govalue[type] * gonum);
    else
        evalred += (StartPositionValue[type][position] + initvalue[type] +
                    govalue[type] * gonum);
}
inline bool isenemy(int pieceNo, bool player) {
    if (!player)  //红
        return pieceNo >= 16;
    else
        return pieceNo < 16;
}
Eval::Eval(PositionStruct& pos) {
    EvalRed = EvalBlack = 0;
    vector<vector<int>> tep[2];
    for (int u = 0; u < 2; ++u) {  //红黑两方
        tep[u] = pos.Predict();
        for (int i = 16 * pos.sdPlayer; i < 16 + 16 * pos.sdPlayer; ++i) {
            if (pos.ucsqPieces[i]) {
                int gonum = 0;
                for (size_t j = 0; j < tep[u][i].size(); ++j) {
                    cnPieceTypes[i];
                    int pieceNo = PosToNo(tep[u][i][j], pos.ucsqPieces);
                    if (pieceNo == -1 || isenemy(pieceNo, pos.sdPlayer))
                        ++gonum;
                }
                cout << gonum << endl;
                calculateEval(pos.ucsqPieces[i], cnPieceTypes[i], EvalRed,
                              EvalBlack, gonum, pos.sdPlayer);
            }
        }
        pos.sdPlayer = !pos.sdPlayer;  //改回来，改回去
        vector<vector<int>>().swap(tep[u]);
    }
}
