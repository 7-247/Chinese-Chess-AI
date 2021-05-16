#pragma once
#include "Position.h"

/*
// 棋盘范围16*16
const int RANK_TOP = 3;
const int RANK_BOTTOM = 12;
const int FILE_LEFT = 3;
const int FILE_RIGHT = 11;
const int ROW = 16;
const int COL = 16;
*/

// 棋盘范围9*10
const int RANK_TOP = 0;
const int RANK_BOTTOM = 9;
const int FILE_LEFT = 0;
const int FILE_RIGHT = 8;
const int ROW = 10;
const int COL = 9;

// 棋子类型对应的中文棋子符号标识

struct ChessBoard {                 //棋盘
   int ucpcSquares[COL * ROW + 1];  // 0~90 0不用
   ChessBoard(PositionStruct&);
   void DrawBoard();
};
