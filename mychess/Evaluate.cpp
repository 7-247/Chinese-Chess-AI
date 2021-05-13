#include "Evaluate.h"
inline void calculateEval(int position, int type, int& evalnum, bool ismid) {
    if (ismid)
        evalnum += EndPositionValue[type][position];
    else
        evalnum += StartPositionValue[type][position];
}
inline int mirrorFlip(int position) {  //对于黑方，请镜像翻转
    return 91 - position;
}

//"帅", "仕", "相", "马", "车", "炮", "兵"
const int PieceTypeValue[] = {0, 1, 1, 3, 6, 3, 1};  //判断开中局还是残局阶段
const int TOTAL_MIDGAME_VALUE = 66;
const int THRE_GAME_VALUE = 20;  //残局阈值
int calculateValue(const int ucsqPieces[],
                   bool sdplayer) {  // 0=红方，1=黑方
    int sum = 0;
    if (sdplayer = true)
        for (int i = 0; i < 16; ++i) {
            if (ucsqPieces[i]) sum += PieceTypeValue[cnPieceTypes[i]];
        }
    else
        for (int i = 16; i < 32; ++i) {
            if (ucsqPieces[i]) sum += PieceTypeValue[cnPieceTypes[i]];
        }
    return sum;
}

Eval::Eval(PositionStruct& pos) {
    EvalRed = EvalBlack = 0;
    // 首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加。
    int midgameValue =
        calculateValue(pos.ucsqPieces, 0) + calculateValue(pos.ucsqPieces, 1);
    /*midgameValue = (2 * TOTAL_MIDGAME_VALUE - midgameValue) * midgameValue /
                   TOTAL_MIDGAME_VALUE;  //二次函数*/

    for (int i = 0; i < 16; ++i)
        if (pos.ucsqPieces[i])
            calculateEval(pos.ucsqPieces[i], cnPieceTypes[i], EvalRed,
                          midgameValue < THRE_GAME_VALUE);
    for (int i = 16; i < 32; ++i)
        if (pos.ucsqPieces[i])
            calculateEval(mirrorFlip(pos.ucsqPieces[i]), cnPieceTypes[i],
                          EvalBlack, midgameValue < THRE_GAME_VALUE);
}
