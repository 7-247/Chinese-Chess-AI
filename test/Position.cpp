#include "Position.h"

inline void PositionStruct::AddPiece(int pos, int no) {  // 在棋盘上放一枚棋子
    nowPos[no] = pos;  //第no号（0~32）棋子的位置（9*10）
}

void PositionStruct::ClearBoard() {  // 清空棋盘
    sdPlayer = 0;
    memset(nowPos, 0, 32 * sizeof(int));
}

// FEN串识别
void PositionStruct::FromFen(const char* const szFen) {
    int i, j, k;
    int pcRed[7];
    int pcBlack[7];
    const char* lpFen;
    // FEN串的识别包括以下几个步骤：
    // 1. 初始化，清空棋盘
    for (i = 0; i < 7; i++) {
        pcRed[i] = FROM_NUM[i];
        pcBlack[i] = pcRed[i] + 16;
    }

    //数组"pcRed[7]"和"pcBlack[7]"分别代表红方和黑方每个兵种即将占有的序号，
    //以"pcRed[7]"为例，由于棋子0到15依次代表“帅仕仕相相马马车车炮炮兵兵兵兵兵”，
    //所以最初应该是"pcRed[7] =
    //{0,1,3,5,7,9,11}"，每添加一个棋子，该项就增加1，
    Count = 0;

    ClearBoard();
    lpFen = szFen;
    // 2. 读取棋盘上的棋子
    i = RANK_TOP;
    j = FILE_LEFT;
    while (*lpFen != ' ') {  //非空格
        if (*lpFen == '/') {
            j = FILE_LEFT;
            ++i;
            if (i > RANK_BOTTOM) {
                break;
            }
        } else if (*lpFen >= '1' && *lpFen <= '9') {
            j += (*lpFen - '0');  //跳到该行下一块找
        } else if (*lpFen >= 'A' && *lpFen <= 'Z') {
            k = FenPiece[*lpFen];
            AddPiece(GetPiecePos(i, j), pcRed[k]++);
            ++j;
        } else if (*lpFen >= 'a' && *lpFen <= 'z') {
            k = FenPiece[*lpFen - 32];
            AddPiece(GetPiecePos(i, j), pcBlack[k]++);
            ++j;
        }
        ++lpFen;
    }
    while (*lpFen == ' ') ++lpFen;
    // 3. 确定轮到哪方走
    if (*lpFen == 'b') {
        sdPlayer = 1;
    }
    if (*lpFen == 0)return;//没有 - - 0 1时
    lpFen += 6;
    while (*lpFen != ' ') ++lpFen;
    while (*lpFen == ' ') ++lpFen;

    // 4. 回合数
    RoundNum = 0;
    while (*lpFen != ' ' && *lpFen != '\0') {
        RoundNum *= 10;
        RoundNum += *lpFen - '0';
        ++lpFen;
    }
    if (*lpFen == '\0') return;     //没有moves
    while (*lpFen == ' ') ++lpFen;  //说明是空格，后面还有moves

    // 5. 处理moves后面的
    while (*lpFen != ' ') {
        ++lpFen;
        if (*lpFen == '\0') return;  //空有moves，没有实质的东西
    }
    while (*lpFen == ' ') ++lpFen;
    moveNode tep = {0, 0, 0};  //开始、结束时子的位置，是否吃子
    while (*lpFen != '\0') {
        tep.src = GetPiecePos('9' - *(lpFen + 1), *lpFen - 'a');
        tep.dst = GetPiecePos('9' - *(lpFen + 3), *(lpFen + 2) - 'a');
        AddPiece(tep.dst, PosToNo(tep.src, nowPos));  //不用考虑吃子
        Moves.push_back(tep);
        sdPlayer = !sdPlayer;//交换走字方
        ++Count;
        lpFen += 4;
        while (*lpFen == ' ') ++lpFen;
    } 
}

void PositionStruct::ChangeBoard(int move) {
    int low = (move & 255), high = ((move - low) >> 8);
    int no = PosToNo(low, nowPos);
    if (no != -1) nowPos[no] = 0;  //被吃了
    nowPos[PosToNo(high, nowPos)] = low;
}

PositionStruct& PositionStruct::operator=(PositionStruct& newchess) {
    memcpy(nowPos , newchess.nowPos,sizeof(int)*32);  // 每个棋子在棋盘上放的位置，0表示被吃了
    //其中0到15表示红子，16到31表示黑子。每方的棋子顺序依次是：
    //帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
    //判断棋子是红子用"pc < 16"，黑子用"pc >= 16"
    sdPlayer = newchess.sdPlayer;  // 轮到谁走，0=红方，1=黑方
    RoundNum = newchess.RoundNum;   // 回合数
    Moves=newchess.Moves;
    Count = newchess.Count;  // Moves[]尾部累积了多少个没吃字的走法
}
