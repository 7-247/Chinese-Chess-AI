#pragma once
#include "Position.h"

// �������Ͷ�Ӧ���������ӷ��ű�ʶ

struct ChessBoard {                  //����
    int ucpcSquares[COL * ROW + 1];  // 0~90 0����
    bool sdPlayer;
    ChessBoard(PositionStruct&);
    void DrawBoard();
    void BoardToFen(char* positionfen);//����תfen��
    void BoardToFenMoves(char* positionfen,int move);//����תfen��

};
