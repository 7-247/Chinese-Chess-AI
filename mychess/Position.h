#pragma once
#include <cstdio>
#include <cstring>
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

// FEN串中棋子标识，注意这个函数只能识别大写字母，因此用小写字母时，首先必须转换为大写
const int FenPiece[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 1, 2, 5,
                        7, 2, 7, 7, 3, 7, 7, 0, 7, 7, 3, 7, 6, 7, 4};

// 起始局面的FEN串
const char* const cszStartFen =
    "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";

// 局面结构
struct PositionStruct {
   int ucsqPieces[32];  // 每个棋子在棋盘上放的位置，0表示被吃了
   //其中0到15表示红子，16到31表示黑子。每方的棋子顺序依次是：
   //帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
   //判断棋子是红子用"pc < 16"，黑子用"pc >= 16"
   //其可行值为33~cb
   bool sdPlayer;  // 轮到谁走，0=红方，1=黑方
   int RoundNum;   // 回合数
   PositionStruct() { FromFen(cszStartFen); }
   PositionStruct(const char* const arr) { FromFen(arr); }
   void ClearBoard();
   void FromFen(const char* const);
   void AddPiece(int, int);         // 在棋盘上放一枚棋子
   void DelPiece(int);              // 从棋盘上拿走一枚棋子
   vector<vector<int>> Generate();  //顶层函数，生成若干种走法

   //请在结构体中删掉下面的无关函数
   vector<vector<int>> possibleMove(const int chessboard[]);  //输出
   vector<int> move_soldier(const int chessboard[], int pos);
   vector<int> move_cannon(const int chessboard[], int pos);
   vector<int> move_cart(const int chessboard[], int pos);
   vector<int> move_horse(const int chessboard[], int pos);
   bool elephant_rule(const int chessboard[], int pos, int move);
   vector<int> move_elephant(const int chessboard[], int pos);
   vector<int> move_knight(const int chessboard[], int pos);
   bool knight_rule(const int chessboard[], int pos, int move);
   bool exist_friend(const int chessboard[], int pos, int move);
   bool exist_all(const int chessboard[], int pos, int move);
   bool exist_enemy(const int chessboard[], int pos, int move);
   bool chief_rule(const int chessboard[], int pos, int move);
   bool judge9(int move_new);
   bool judgeRiver(int move_new);
   bool horse_rule(const int chessboard[], int pos, int move);
   vector<int> move_chief(const int chessboard[], int pos);
};
int GetPiecePos(int i, int j);  //二维转一维
