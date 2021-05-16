#pragma once
#include "Position.h"

/*
// ∆Â≈Ã∑∂Œß16*16
const int RANK_TOP = 3;
const int RANK_BOTTOM = 12;
const int FILE_LEFT = 3;
const int FILE_RIGHT = 11;
const int ROW = 16;
const int COL = 16;
*/


struct ChessBoard {                 //∆Â≈Ã
   int ucpcSquares[COL * ROW + 1];  // 0~90 0≤ª”√
   ChessBoard(PositionStruct&);
   void DrawBoard();
};
