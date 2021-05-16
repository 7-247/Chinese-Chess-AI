#include "ChessBoard.h"
// 绘图
inline const char* PIECE_BYTE_IN_CHINESE(int pt, bool type) {
   if (type == true)
      return cszPieceBytesInChineseRed[pt];
   else
      return cszPieceBytesInChineseBlack[pt];
}

ChessBoard::ChessBoard(PositionStruct& pos) {
   memset(ucpcSquares, -1, (ROW * COL + 1) * sizeof(int));
   for (int i = 0; i < 32; ++i) {
      ucpcSquares[pos.ucsqPieces[i]] =
          i;  // pos.ucsqPieces[i]=0时，代表序号为i的棋子被吃了
      //无需像下面这样写，因为棋盘的第零号位并我们不使用，可以省去判断是否为0的if
      // int tep = pos.ucsqPieces[i];
      // if(tep)ucpcSquares[tep] = i;
   }
}

void ChessBoard::DrawBoard() {
   {
      int pc;
      const char* c;
      for (int i = RANK_TOP; i <= RANK_BOTTOM; ++i) {
         for (int j = FILE_LEFT; j <= FILE_RIGHT; ++j) {
            pc = ucpcSquares[GetPiecePos(i, j)];  //根据棋子的位置找棋子的序号
            if (pc == -1) {
               printf(" .");
            } else {
               c = PIECE_BYTE_IN_CHINESE(cnPieceTypes[pc], pc < 16);
               printf("%s", c);
            }
         }
         printf(" %d\n", ROW - i - 1);
      }
      printf(" a b c d e f g h i\n");
   }
}
