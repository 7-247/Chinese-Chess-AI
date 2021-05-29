#include "ChessBoard.h"
#include "Ucci.h"
#include "Position.h"
// 绘图
inline const char* PIECE_BYTE_IN_CHINESE(int pt, bool type) {
	if (type == true)
		return cszPieceBytesInChineseRed[pt];
	else
		return cszPieceBytesInChineseBlack[pt];
}

ChessBoard::ChessBoard(PositionStruct& pos) {
	memset(ucpcSquares, -1, (ROW * COL + 1) * sizeof(int));
	sdPlayer = pos.sdPlayer;
	for (int i = 0; i < 32; ++i) {
		ucpcSquares[pos.nowPos[i]] = i;  // pos.nowPos[i]=0时，代表序号为i的棋子被吃了
		//无需像下面这样写，因为棋盘的第零号位并我们不使用，可以省去判断是否为0的if
		// int tep = pos.nowPos[i];
		// if(tep)ucpcSquares[tep] = i;
	}
}

void ChessBoard::DrawBoard() {
	{
		int pc;
		const char* c;
		for (int i = RANK_TOP; i <= RANK_BOTTOM; ++i) {
			for (int j = FILE_LEFT; j <= FILE_RIGHT; ++j) {
				pc = ucpcSquares[GetPiecePos(i,
					j)];  //根据棋子的位置找棋子的序号
				if (pc == -1) {
					printf(" .");
				}
				else {
					c = PIECE_BYTE_IN_CHINESE(cnPieceTypes[pc], pc < 16);
					printf("%s", c);
				}
			}
			printf(" %d\n", ROW - i - 1);
		}
		printf(" a b c d e f g h i\n");
	}
}

const char PieceBytes[] = "KAABBNNRRCCPPPPPkaabbnnrrccppppp";

void ChessBoard::BoardToFen(char* positionfen) {//局面转fen串
//demo: rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1    
	memset(positionfen, 0, 1000);
	memcpy(positionfen, "position fen ", 14);
	int cur = 13;
	for (int i = 0; i < ROW; ++i) {
		int cnt = 0;
		for (int j = 0; j < COL; ++j)
		{
			if (ucpcSquares[i * COL + j + 1] == -1)++cnt;
			else {
				if (cnt)positionfen[cur++] = cnt + '0';
				cnt = 0;
				positionfen[cur++] = PieceBytes[ucpcSquares[i * COL + j + 1]];
			}

		}
		if (cnt)positionfen[cur++] = cnt + '0';
		positionfen[cur++] = '/';
	}
	--cur;//最后一层不必加 '/'
	positionfen[cur++] = ' ';
	if (sdPlayer == 0)positionfen[cur++] = 'r';
	else positionfen[cur++] = 'b';
	positionfen[cur++] = ' '; positionfen[cur++] = '-'; positionfen[cur++] = ' '; positionfen[cur++] = '-';
	positionfen[cur++] = ' '; positionfen[cur++] = '0'; positionfen[cur++] = ' '; positionfen[cur++] = '1';
	positionfen[cur++] = ' ';positionfen[cur++] = 'm'; positionfen[cur++] = 'o'; positionfen[cur++] = 'v'; 
	positionfen[cur++] = 'e'; positionfen[cur++] = 's';
	//目前就不考虑moves了
}

void ChessBoard::BoardToFenMoves(char* positionfen, int bestmove) {//局面转fen串
//demo:position fen rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1 moves h2e2
	int cur = 0;
	int low = bestmove & 255, high = (bestmove - low) >> 8;
	high -= 1, low -= 1;
	char tepmove[5] = { 0 };
	tepmove[0] = high % 9 + 'a', tepmove[1] = 9 - high / 9+'0';
	tepmove[2] = low % 9 + 'a', tepmove[3] = 9 - low / 9 + '0';
	while (positionfen[++cur] != 0);
	positionfen[cur++] = ' ';
	positionfen[cur++] = tepmove[0]; positionfen[cur++] = tepmove[1];
	positionfen[cur++] = tepmove[2]; positionfen[cur++] = tepmove[3];
}