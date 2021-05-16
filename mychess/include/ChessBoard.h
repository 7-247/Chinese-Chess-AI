#pragma once
#include "Position.h"

/*
// ���̷�Χ16*16
const int RANK_TOP = 3;
const int RANK_BOTTOM = 12;
const int FILE_LEFT = 3;
const int FILE_RIGHT = 11;
const int ROW = 16;
const int COL = 16;
*/

// ���̷�Χ9*10
const int RANK_TOP = 0;
const int RANK_BOTTOM = 9;
const int FILE_LEFT = 0;
const int FILE_RIGHT = 8;
const int ROW = 10;
const int COL = 9;

// �������Ͷ�Ӧ���������ӷ��ű�ʶ

struct ChessBoard {                 //����
   int ucpcSquares[COL * ROW + 1];  // 0~90 0����
   ChessBoard(PositionStruct&);
   void DrawBoard();
};
