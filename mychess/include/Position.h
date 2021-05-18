#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

/*
    棋子序号对应的棋子类型
    0到15表示红子，16到31表示黑子。每方的棋子顺序依次是：
    每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
    判断棋子是红子用"pc < 16"，黑子用"pc >= 16"
 */
static const int cnPieceTypes[32] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
                                     6, 6, 6, 6, 6, 0, 1, 1, 2, 2, 3,
                                     3, 4, 4, 5, 5, 6, 6, 6, 6, 6};

// 棋子类型对应的棋子符号
const char* const cszPieceBytes = "KABNRCP";

const int FROM_NUM[] = {0, 1, 3, 5, 7, 9, 11};  // 每种子力的开始序号和结束序号

const char* const cszPieceBytesInChineseBlack[7] = {"将", "士", "象", "碼",
                                                    "車", "砲", "卒"};
const char* const cszPieceBytesInChineseRed[7] = {"帅", "仕", "相", "马",
                                                  "车", "炮", "兵"};

// FEN串中棋子标识，注意这个函数只能识别大写字母，因此用小写字母时，首先必须转换为大写
const int FenPiece[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1, 2, 5,
                        7, 2, 7, 7, 3, 7, 7, 0, 7, 7, 3, 7, 6, 7, 4};

// 棋盘范围9*10
const int RANK_TOP = 0;
const int RANK_BOTTOM = 9;
const int FILE_LEFT = 0;
const int FILE_RIGHT = 8;
const int ROW = 10;
const int COL = 9;
// 起始局面的FEN串
const char* const cszStartFen =
    "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
struct moveNode {
    int src, dst;
    bool iseat;
    bool operator==(moveNode& node1);
};
// 局面结构
struct PositionStruct {
    int ucsqPieces[32];  // 每个棋子在棋盘上放的位置，0表示被吃了
    //其中0到15表示红子，16到31表示黑子。每方的棋子顺序依次是：
    //帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
    //判断棋子是红子用"pc < 16"，黑子用"pc >= 16"
    //其可行值为33~cb
    bool sdPlayer;  // 轮到谁走，0=红方，1=黑方
    int RoundNum;   // 回合数
    vector<moveNode> Moves;
    int Count;  // Moves[]尾部累积了多少个没吃字的走法
    PositionStruct() { FromFen(cszStartFen); }
    PositionStruct(const char* const arr) { FromFen(arr); }
    void PositionInit(const char* const arr) { FromFen(arr); }
    void ClearBoard();
    void FromFen(const char* const);
    void AddPiece(int, int);         // 在棋盘上放一枚棋子
    void DelPiece(int);              // 从棋盘上拿走一枚棋子
    vector<vector<int>> Generate();  //顶层函数，生成若干种走法
    int Repeat();
    vector<vector<int>> Predict();  //顶层函数，生成若干种走法(保护，威胁)
};
inline void PositionStruct::DelPiece(int no) {  // 从棋盘上拿走一枚棋子
    ucsqPieces[no] = 0;
}
inline int PosToNo(int pos, int ucsqPieces[]) {
    for (int i = 0; i < 32; ++i)
        if (pos == ucsqPieces[i]) return i;
    return -1;
}
inline int GetPiecePos(int i, int j) {
    return i * COL + j + 1;  //棋盘中的0空着，用1~90
}