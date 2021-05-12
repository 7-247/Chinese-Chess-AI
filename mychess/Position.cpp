#include "ChessBoard.h"

inline void PositionStruct::AddPiece(int pos, int no) {  // 在棋盘上放一枚棋子
   ucsqPieces[no] = pos;  //第no号（0~32）棋子的位置（9*10）
}
inline void PositionStruct::DelPiece(int no) {  // 从棋盘上拿走一枚棋子
   ucsqPieces[no] = 0;
}

void PositionStruct::ClearBoard() {  // 清空棋盘
   sdPlayer = 0;
   memset(ucsqPieces, 0, 32 * sizeof(int));
}

inline int GetPiecePos(int i, int j) {
   return i * COL + j + 1;  //棋盘中的0空着，用1~90
}

// FEN串识别
void PositionStruct::FromFen(const char* const szFen) {
   int i, j, k;
   int pcRed[7];
   int pcBlack[7];
   const char* lpFen;
   // FEN串的识别包括以下几个步骤：
   // 1. 初始化，清空棋盘
   for (i = 0; i < 7; i++) {
      pcRed[i] = FROM_NUM[i];
      pcBlack[i] = pcRed[i] + 16;
   }

   //数组"pcRed[7]"和"pcBlack[7]"分别代表红方和黑方每个兵种即将占有的序号，
   //以"pcRed[7]"为例，由于棋子0到15依次代表“帅仕仕相相马马车车炮炮兵兵兵兵兵”，
   //所以最初应该是"pcRed[7] =
   //{0,1,3,5,7,9,11}"，每添加一个棋子，该项就增加1，

   ClearBoard();
   lpFen = szFen;
   // 2. 读取棋盘上的棋子
   i = RANK_TOP;
   j = FILE_LEFT;
   while (*lpFen != ' ') {  //非空格
      if (*lpFen == '/') {
         j = FILE_LEFT;
         ++i;
         if (i > RANK_BOTTOM) {
            break;
         }
      } else if (*lpFen >= '1' && *lpFen <= '9') {
         j += (*lpFen - '0');  //跳到该行下一块找
      } else if (*lpFen >= 'A' && *lpFen <= 'Z') {
         k = FenPiece[*lpFen];
         AddPiece(GetPiecePos(i, j), pcRed[k]++);
         ++j;
      } else if (*lpFen >= 'a' && *lpFen <= 'z') {
         k = FenPiece[*lpFen - 32];
         AddPiece(GetPiecePos(i, j), pcBlack[k]++);
         ++j;
      }
      ++lpFen;
   }
   ++lpFen;
   // 3. 确定轮到哪方走
   if (*lpFen == 'b') {
      sdPlayer = 1;
   }
   lpFen += 6;
   while (*lpFen != ' ') {
      ++lpFen;
   }
   ++lpFen;
   // 4. 回合数
   RoundNum = 0;
   while (*lpFen != '\0') {
      RoundNum *= 10;
      RoundNum += *lpFen - '0';
      ++lpFen;
   }
}
