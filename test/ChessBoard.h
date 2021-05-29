#pragma once
#include "Position.h"

// 棋子类型对应的中文棋子符号标识

struct ChessBoard {                  //棋盘
    int ucpcSquares[COL * ROW + 1];  // 0~90 0不用
    bool sdPlayer;
    ChessBoard(PositionStruct&);
    void DrawBoard();
    void BoardToFen(char* positionfen);//局面转fen串
    void BoardToFenMoves(char* positionfen,int move);//局面转fen串

};
