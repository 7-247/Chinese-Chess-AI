#include "Position.h"

inline void PositionStruct::AddPiece(int pos, int type) { // 在棋盘上放一枚棋子
	ucsqPieces[type] = pos;
}
inline void PositionStruct::DelPiece(int type) { // 从棋盘上拿走一枚棋子
	ucsqPieces[type] = 0;
}

#include<string.h>
void PositionStruct::ClearBoard() {         // 清空棋盘
	sdPlayer = 0;
	memset(ucsqPieces, 0, 48);
}


// FEN串识别
void PositionStruct::FromFen(const char* const szFen)
{
	int i, j, k;
	int pcWhite[7];
	int pcBlack[7];
	const char* lpFen;
	// FEN串的识别包括以下几个步骤：
	// 1. 初始化，清空棋盘
	for (i = 0; i < 7; i++) {
		pcWhite[i] = FROM_NUM[i];
		pcBlack[i] = pcWhite[i] + 16;
	}
	/* 
		数组"pcWhite[7]"和"pcBlack[7]"分别代表红方和黑方每个兵种即将占有的序号，
		以"pcWhite[7]"为例，由于棋子16到31依次代表“帅仕仕相相马马车车炮炮兵兵兵兵兵”，
		所以最初应该是"pcWhite[7] = {16, 17, 19, 21, 23, 25, 27}"，每添加一个棋子，该项就增加1，
	*/
	ClearBoard();
	lpFen = szFen;
	// 2. 读取棋盘上的棋子
	i = RANK_TOP;
	j = FILE_LEFT;
	while (*lpFen != ' ') {//非空格
		if (*lpFen == '/') {
			j = FILE_LEFT;
			++i;
			if (i > RANK_BOTTOM) {
				break;
			}
		}
		else if (*lpFen >= '1' && *lpFen <= '9') {
			j += (*lpFen - '0');//跳到该行下一块找
		}
		else if (*lpFen >= 'A' && *lpFen <= 'Z') {
				k = FenPiece[*lpFen];
				AddPiece(((i << 4) + j), pcWhite[k]++);
				++j;
		}
		else if (*lpFen >= 'a' && *lpFen <= 'z') {
				k = FenPiece[*lpFen - 32];
				AddPiece(((i << 4) + j), pcBlack[k]++);
				++j;
		}
		++lpFen;
	}
	++lpFen;
	// 3. 确定轮到哪方走
	if (*lpFen == 'b') {
		sdPlayer = 1;
	}

}

// 绘图
inline const char* PIECE_BYTE_IN_CHINESE(int pt, bool type) {
	if (type == true)
		return cszPieceBytesInChineseRed[pt];
	else
		return cszPieceBytesInChineseBlack[pt];
}

ChessBoard::ChessBoard(PositionStruct& pos) {
	memset(ucpcSquares, -1, 256);
	for (int i = 0; i < 32; ++i)
		ucpcSquares[pos.ucsqPieces[i]] = i;
}


#include <cstdio>
void ChessBoard::DrawBoard()
{
	{
		int pc;
		const char* c;
		for (int i = RANK_TOP; i <= RANK_BOTTOM; ++i) {
			for (int j = FILE_LEFT; j <= FILE_RIGHT; ++j) {
				pc = ucpcSquares[(i << 4) + j];//i*16+j 找棋子的位置
				if (pc == -1) {
					printf(" .");
				}
				else {
					c = PIECE_BYTE_IN_CHINESE(cnPieceTypes[pc], pc < 16);
					printf("%s", c);
				}
			}
			printf(" %d\n", 12 - i);
		}
		printf(" a b c d e f g h i\n");
	}
}
