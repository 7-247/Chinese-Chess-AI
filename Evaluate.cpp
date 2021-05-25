#include "Evaluate.h"

#include <algorithm>
bool isend = false;
bool isattackred = false;
bool isattackblack = false;
//"将","士","象","碼","車","砲","卒" 固有价值
const int initvalue[] = { 65535, 20, 20, 0, 50, 20, 7 };
//"将","士","象","碼","車","砲","卒" 机动性价值（每多一步走法所加的分）
const int govalue[] = { 0, 0, 0, 3, 1, 0, 5 };

inline int mirrorFlip(int position) {  //对于黑方，请镜像翻转
    return 91 - position;
}
inline int calculateBlackEval(int position, int type, int gonum) {
    return (
        PositionValue[isend * 2 + isattackblack][type][mirrorFlip(position)] +
        initvalue[type] + govalue[type] * gonum);
}
inline int calculateRedEval(int position, int type, int gonum) {
    return (PositionValue[isend * 2 + isattackred][type][position] +
        initvalue[type] + govalue[type] * gonum);
}
inline int calculateEval(int position, int type, int gonum, bool isblack) {
    return (PositionValue[isend * 2 + (isblack ? isattackblack : isattackred)]
        [type][(91 - 2 * position) * isblack + position] +
        initvalue[type] + govalue[type] * gonum);
}

int ThreatenValue[32][17] = { 0 };  //敌方的威胁
int ProtectValue[32][17] = { 0 };   //己方的保护
int gonum[32] = { 0 };              //机动性
vector<vector<int>> tep[2];
int threatnum[32] = { 0 };
int protectnum[32] = { 0 };
inline bool Threaten(int No, int nowPos[]) {
    //一轮交换我亏了,保护不过来返回true
    if (ProtectValue[No][0] == 0 && ThreatenValue[No][0] > 0)  //有威胁无保护
        return true;
    else if (ThreatenValue[No][0] == 0)  //威胁为0
        return false;
    else {  //有保护有威胁
        // memset(threatnum, 0, sizeof(threatnum));
        // memset(protectnum, 0, sizeof(protectnum));
        int ThreatenvalueSum = 0,
            ProtectvalueSum = calculateEval(nowPos[No], cnPieceTypes[No],
                gonum[No], No >= 16);
        //在威胁与保护相等时最大保护棋子的价值是不必付出的
        if (No < 16) {  //红方棋子
            for (int i = 1; i <= ThreatenValue[No][0]; ++i) {
                threatnum[i - 1] =
                    calculateBlackEval(nowPos[ThreatenValue[No][i]],
                        cnPieceTypes[ThreatenValue[No][i]],
                        gonum[ThreatenValue[No][i]]);
                ThreatenvalueSum += threatnum[i - 1];
            }
            for (int i = 1; i <= ProtectValue[No][0]; ++i) {
                protectnum[i - 1] =
                    calculateRedEval(nowPos[ProtectValue[No][i]],
                        cnPieceTypes[ProtectValue[No][i]],
                        gonum[ProtectValue[No][i]]);
                ProtectvalueSum += protectnum[i - 1];
            }
        }
        else {  //黑方棋子
            for (int i = 1; i <= ThreatenValue[No][0]; ++i) {
                threatnum[i - 1] =
                    calculateRedEval(nowPos[ThreatenValue[No][i]],
                        cnPieceTypes[ThreatenValue[No][i]],
                        gonum[ThreatenValue[No][i]]);
                ThreatenvalueSum += threatnum[i - 1];
            }
            for (int i = 1; i <= ProtectValue[No][0]; ++i) {
                protectnum[i - 1] =
                    calculateBlackEval(nowPos[ProtectValue[No][i]],
                        cnPieceTypes[ProtectValue[No][i]],
                        gonum[ProtectValue[No][i]]);
                ProtectvalueSum += protectnum[i - 1];
            }
        }
        sort(threatnum, threatnum + ThreatenValue[No][0]);
        sort(protectnum, protectnum + ProtectValue[No][0]);
        if (ThreatenValue[No][0] > ProtectValue[No][0]) {  //剩下攻击方
            int tepnum =
                ThreatenValue[No][0] - ProtectValue[No][0];  //攻击方余下数量
            for (int j = ThreatenValue[No][0] - tepnum;
                j < ThreatenValue[No][0]; ++j) {
                ThreatenvalueSum -= threatnum[j];  //不必付出的代价
            }
        }
        else {  //剩下防守方
            int tepnum = ProtectValue[No][0] + 1 -
                ThreatenValue[No][0];  //防守方余下数量
            for (int j = ProtectValue[No][0] - tepnum; j < ProtectValue[No][0];
                ++j) {
                ProtectvalueSum -= protectnum[j];  //不必付出的代价
            }
        }
        if (ThreatenvalueSum > ProtectvalueSum) {  //攻击方付出了更大代价
            return false;
        }
        else
            return true;
    }
}

//"帅", "仕", "相", "马", "车", "炮", "兵"
const int PieceTypeValue[] = { 0, 1, 1, 3, 6, 3, 1 };
//判断开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加
const int TOTAL_MIDGAME_VALUE = 66;
const int THRE_GAME_VALUE = 30;  //残局阈值
int calculateValue(const int nowPos[],
    bool sdplayer) {  // 0=红方，1=黑方
    int sum = 0;
    if (sdplayer == false) {
        for (int i = 0; i < 16; ++i) {
            if (nowPos[i]) sum += PieceTypeValue[cnPieceTypes[i]];
        }
    }
    else {
        for (int i = 16; i < 32; ++i) {
            if (nowPos[i]) sum += PieceTypeValue[cnPieceTypes[i]];
        }
    }
    return sum;
}

//计算各种过河棋子的数量，按照车马2炮兵1相加。
//"帅", "仕", "相", "马", "车", "炮", "兵"
const int PieceTypeAttackValue[] = { 0, 0, 0, 2, 2, 1, 1 };

void Eval::EvalInit(PositionStruct& pos) {
    EvalRed = EvalBlack = 0;
    vector<vector<int>>().swap(tep[0]);
    vector<vector<int>>().swap(tep[1]);
    memset(ThreatenValue, 0, sizeof(ThreatenValue));
    memset(ProtectValue, 0, sizeof(ProtectValue));
    memset(gonum, 0, sizeof(gonum));
    isend = false;
    isattackred = false;
    isattackblack = false;
    // 判断局势处于开中局还是残局阶段。
    int midgameValue =
        calculateValue(pos.nowPos, 0) + calculateValue(pos.nowPos, 1);
    if (midgameValue < THRE_GAME_VALUE) isend = true;
    // 判断各方是否处于进攻状态.
    int nRedAttacks = 0, nBlackAttacks = 0;
    for (int i = 0; i < 16; ++i)
        if (pos.nowPos[i] <= 45 && pos.nowPos > 0)
            nRedAttacks += PieceTypeAttackValue[cnPieceTypes[i]];
    for (int i = 16; i < 32; ++i)
        if (pos.nowPos[i] > 45)
            nBlackAttacks += PieceTypeAttackValue[cnPieceTypes[i]];
    if (pos.nowPos[9] % 9 == 5 || pos.nowPos[10] % 9 == 5)
        nRedAttacks += 1;  //中炮
    if (pos.nowPos[25] % 9 == 5 || pos.nowPos[26] % 9 == 5)
        nRedAttacks += 1;  //中炮

    if (nRedAttacks > 5) isattackred = 1;
    if (nBlackAttacks > 5) isattackblack = 1;

    tep[pos.sdPlayer] = pos.Predict();  // 0为红，1为黑
    pos.sdPlayer = !pos.sdPlayer;
    tep[pos.sdPlayer] = pos.Predict();
    pos.sdPlayer = !pos.sdPlayer;  //换回来

    for (int i = 0; i < 16; ++i) {  //考虑红方
        if (pos.nowPos[i]) {
            for (size_t j = 0; j < tep[0][i].size(); ++j) {
                int pieceNo = PosToNo(tep[0][i][j], pos.nowPos);
                if (pieceNo == -1)  //红方所有走法的目标位置
                    ++gonum[i];
                else if (pieceNo >= 16) {  //黑
                    ThreatenValue[pieceNo][++ThreatenValue[pieceNo][0]] = i;
                    //记录威胁序号
                    //黑方第pieceNo号棋子收到了红方i号棋子的威胁
                    ++gonum[i];
                }
                else {  // 无需额外判断了，必定是友方
                       // if(isfriend(pieceNo,po.sdPlayer))
                    ProtectValue[pieceNo][++ProtectValue[pieceNo][0]] = i;
                    //记录保护序号
                    //红方第pieceNo号棋子收到了红方i号棋子的保护
                }
            }
        }
    }
    for (int i = 16; i < 32; ++i) {  //黑
        if (pos.nowPos[i]) {
            for (size_t j = 0; j < tep[1][i].size(); ++j) {
                int pieceNo = PosToNo(tep[1][i][j], pos.nowPos);
                if (pieceNo == -1)
                    ++gonum[i];
                else if (pieceNo < 16) {  //红
                    ThreatenValue[pieceNo][++ThreatenValue[pieceNo][0]] = i;
                    //记录威胁序号
                    //红方第pieceNo号棋子收到了黑方i号棋子的威胁
                    ++gonum[i];
                }
                else {  // 无需额外判断了，必定是友方
                       // if(isfriend(pieceNo,po.sdPlayer))
                    ProtectValue[pieceNo][++ProtectValue[pieceNo][0]] = i;
                    //记录保护序号
                    //黑方第pieceNo号棋子收到了黑方i号棋子的保护
                }
            }
        }
    }

    if (pos.sdPlayer == 0) {  //假如现在是红方准备走棋
        int MaxthreatenNo = -1, MaxthreatenValue = -999999;
        //吃或保护只能选一个bool eat = 0, protect = 0;
        for (int i = 0; i < 16; ++i) {
            if (pos.nowPos[i]) {
                int value = calculateRedEval(pos.nowPos[i], cnPieceTypes[i],
                    gonum[i]);
                if (Threaten(i, pos.nowPos)) {  //保护不过来了
                    if (value > MaxthreatenValue) {
                        if (MaxthreatenNo != -1)  //已经发现过了
                            EvalRed -= (MaxthreatenValue >> 1);  //补扣 扣一半
                        MaxthreatenNo = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue = value;
                    }
                }
                EvalRed += value;
                //因为此时我方是先手，故不必扣除此处分数，后面补扣
            }
        }
        int MaxthreatenNo2 = -1, MaxthreatenValue2 = -999999;
        for (int i = 16; i < 32; ++i) {
            if (pos.nowPos[i]) {
                int value = calculateBlackEval(pos.nowPos[i],
                    cnPieceTypes[i], gonum[i]);
                if (Threaten(i, pos.nowPos)) {
                    //保护不过来,即威胁的子的价值和低于你自己本身的价值+保护你的子的价值
                    if (value > MaxthreatenValue2) {
                        if (MaxthreatenNo2 != -1)  //已经发现过了
                            EvalBlack += MaxthreatenValue2;  //补加
                        MaxthreatenNo2 = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue2 = value;
                    }
                }
                else  //此处不加，后面补加
                    EvalBlack += value;
            }
        }
        if (MaxthreatenNo2 != -1)EvalBlack += (MaxthreatenValue2 >> 1);
        if (MaxthreatenNo2 != -1 && MaxthreatenNo != -1) {  //两者只能选其一
            bool onlyforone = true;  //两者只能选其一
            for (int i = 1; i <= ThreatenValue[MaxthreatenNo2][0]; ++i)
                if (ThreatenValue[MaxthreatenNo2][i] == MaxthreatenNo) {
                    onlyforone = false;
                    break;
                }
            if (onlyforone)
                EvalRed -=
                min((MaxthreatenValue >> 1), (MaxthreatenValue2 >> 1));
        }
    }
    else {  //假如现在是黑方准备走棋
        int MaxthreatenNo = -1, MaxthreatenValue = -999999;
        for (int i = 16; i < 32; ++i) {
            if (pos.nowPos[i]) {
                int value = calculateBlackEval(pos.nowPos[i],
                    cnPieceTypes[i], gonum[i]);
                if (Threaten(i, pos.nowPos)) {  //保护不过来了
                    if (value > MaxthreatenValue) {
                        if (MaxthreatenNo != -1)  //已经发现过了
                            EvalBlack -= (MaxthreatenValue >> 1);  //补扣 扣一半
                        MaxthreatenNo = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue = value;
                    }
                }
                EvalBlack += value;
                //因为此时我方是先手，故不必扣除此处分数，后面补扣
            }
        }
        int MaxthreatenNo2 = -1, MaxthreatenValue2 = -999999;
        for (int i = 0; i < 16; ++i) {
            if (pos.nowPos[i]) {
                int value = calculateRedEval(pos.nowPos[i], cnPieceTypes[i],
                    gonum[i]);
                if (Threaten(i, pos.nowPos)) {  //保护不过来了
                    if (value > MaxthreatenValue2) {
                        if (MaxthreatenNo2 != -1)          //已经发现过了
                            EvalRed += MaxthreatenValue2;  //补加
                        MaxthreatenNo2 = i;  //记录值最大的那个，让其得以逃逸
                        MaxthreatenValue2 = value;
                    }
                }
                else  //此处不加，后面补加
                    EvalRed += value;
            }
        }
        if (MaxthreatenNo2 != -1)EvalRed += (MaxthreatenValue2 >> 1);  //补加一半
        if (MaxthreatenNo2 != -1 && MaxthreatenNo != -1) {
            bool onlyforone = true;  //两者只能选其一
            for (int i = 1; i <= ThreatenValue[MaxthreatenNo2][0]; ++i)
                if (ThreatenValue[MaxthreatenNo2][i] == MaxthreatenNo) {
                    onlyforone = false;
                    break;
                }
            if (onlyforone)
                EvalRed +=
                min((MaxthreatenValue >> 1), (MaxthreatenValue2 >> 1));
        }
    }
}
