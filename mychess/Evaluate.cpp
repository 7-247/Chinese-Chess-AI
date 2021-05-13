#include "Evaluate.h"

// 空头炮的威胁分值，指标是对红方来说的行号(即黑方要用15去减)，大体上空头炮位置越高威胁越大。进入残局时，该值要相应减少。
static const int cvlHollowThreat[16] = {0,  0,  0,  0,  0,  0, 60, 65,
                                        70, 75, 80, 80, 80, 0, 0,  0};

// 炮镇窝心马的威胁分值，指标同上，大体上高度越低威胁越大，没有窝心马时可取四分之一。进入残局时，取值似乎不应变化。
static const int cvlCentralThreat[16] = {0,  0,  0,  0,  0,  0, 50, 45,
                                         40, 35, 30, 30, 30, 0, 0,  0};

// 沉底炮的威胁分值，指标是列号，大体上越靠近边线威胁越大。威胁减少时，该值要相应减少。
static const int cvlBottomThreat[16] = {0, 0, 0,  40, 30, 0, 0, 0,
                                        0, 0, 30, 40, 0,  0, 0, 0};

//"帅", "仕", "相", "马", "车", "炮", "兵"
const int PieceTypeValue[] = {0, 1, 1, 3, 6, 3, 1};
//判断开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加
const int TOTAL_MIDGAME_VALUE = 66;
const int THRE_GAME_VALUE = 20;  //残局阈值

inline void calculateEval(int position, int type, int& evalnum, bool ismid) {
    if (ismid)
        evalnum += EndPositionValue[type][position];
    else
        evalnum += StartPositionValue[type][position];
}
inline int mirrorFlip(int position) {  //对于黑方，请镜像翻转
    return 91 - position;
}

int calculateValue(const int ucsqPieces[],
                   bool sdplayer) {  // 0=红方，1=黑方
    int sum = 0;
    if (sdplayer == false) {
        for (int i = 0; i < 16; ++i) {
            if (ucsqPieces[i]) sum += PieceTypeValue[cnPieceTypes[i]];
        }
    } else {
        for (int i = 16; i < 32; ++i) {
            if (ucsqPieces[i]) sum += PieceTypeValue[cnPieceTypes[i]];
        }
    }
    return sum;
}

Eval::Eval(PositionStruct& pos) {
    EvalRed = EvalBlack = 0;
    // 首先判断局势处于开中局还是残局阶段。
    int midgameValue =
        calculateValue(pos.ucsqPieces, 0) + calculateValue(pos.ucsqPieces, 1);
    printf("%d %d\n", calculateValue(pos.ucsqPieces, 0),
           calculateValue(pos.ucsqPieces, 1));
    for (int i = 0; i < 16; ++i)
        if (pos.ucsqPieces[i])
            calculateEval(pos.ucsqPieces[i], cnPieceTypes[i], EvalRed,
                          midgameValue < THRE_GAME_VALUE);
    for (int i = 16; i < 32; ++i)
        if (pos.ucsqPieces[i])
            calculateEval(mirrorFlip(pos.ucsqPieces[i]), cnPieceTypes[i],
                          EvalBlack, midgameValue < THRE_GAME_VALUE);
}
