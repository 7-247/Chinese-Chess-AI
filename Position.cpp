#include "Position.h"

inline void PositionStruct::AddPiece(int pos, int type) { // �������Ϸ�һö����
	ucsqPieces[type] = pos;
}
inline void PositionStruct::DelPiece(int type) { // ������������һö����
	ucsqPieces[type] = 0;
}

#include<string.h>
void PositionStruct::ClearBoard() {         // �������
	sdPlayer = 0;
	memset(ucsqPieces, 0, 48);
}


// FEN��ʶ��
void PositionStruct::FromFen(const char* const szFen)
{
	int i, j, k;
	int pcWhite[7];
	int pcBlack[7];
	const char* lpFen;
	// FEN����ʶ��������¼������裺
	// 1. ��ʼ�����������
	for (i = 0; i < 7; i++) {
		pcWhite[i] = FROM_NUM[i];
		pcBlack[i] = pcWhite[i] + 16;
	}
	/* 
		����"pcWhite[7]"��"pcBlack[7]"�ֱ����췽�ͺڷ�ÿ�����ּ���ռ�е���ţ�
		��"pcWhite[7]"Ϊ������������16��31���δ���˧�����������������ڱ�������������
		�������Ӧ����"pcWhite[7] = {16, 17, 19, 21, 23, 25, 27}"��ÿ���һ�����ӣ����������1��
	*/
	ClearBoard();
	lpFen = szFen;
	// 2. ��ȡ�����ϵ�����
	i = RANK_TOP;
	j = FILE_LEFT;
	while (*lpFen != ' ') {//�ǿո�
		if (*lpFen == '/') {
			j = FILE_LEFT;
			++i;
			if (i > RANK_BOTTOM) {
				break;
			}
		}
		else if (*lpFen >= '1' && *lpFen <= '9') {
			j += (*lpFen - '0');//����������һ����
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
	// 3. ȷ���ֵ��ķ���
	if (*lpFen == 'b') {
		sdPlayer = 1;
	}

}

// ��ͼ
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
				pc = ucpcSquares[(i << 4) + j];//i*16+j �����ӵ�λ��
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
