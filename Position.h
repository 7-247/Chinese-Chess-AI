#pragma once

// 棋盘范围
const int RANK_TOP = 3;
const int RANK_BOTTOM = 12;
const int FILE_LEFT = 3;
const int FILE_RIGHT = 11;

/* 
	棋子序号对应的棋子类型
	0到15表示红子，16到31表示黑子。每方的棋子顺序依次是：
	每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
	判断棋子是红子用"pc < 16"，黑子用"pc >= 16"
 */
static const int cnPieceTypes[32] = {
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

const int FROM_NUM[] = { 0,1,3,5,7,9,11 };// 每种子力的开始序号和结束序号

// FEN串中棋子标识，注意这个函数只能识别大写字母，因此用小写字母时，首先必须转换为大写
const int FenPiece[] = {
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	1,2,5,7,2,7,7,3,7,7,0,7,7,3,7,6,7,4 
};

// 起始局面的FEN串
const char* const cszStartFen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";

// 棋子类型对应的棋子符号
const char* const cszPieceBytes = "KABNRCP";

// 棋子类型对应的中文棋子符号标识
const char* const cszPieceBytesInChineseBlack[7] = { "将","士","象","碼","車","砲","卒" };
const char* const cszPieceBytesInChineseRed[7] = { "帅","仕","相","马","车","炮","兵" };


// 局面结构
struct PositionStruct {
	int ucsqPieces[32];		// 每个棋子在棋盘上放的位置，0表示被吃了
	//其中0到15表示红子，16到31表示黑子。每方的棋子顺序依次是：
	//帅仕仕相相马马车车炮炮兵兵兵兵兵(将	士士象象马马车车炮炮卒卒卒卒卒)
	//判断棋子是红子用"pc < 16"，黑子用"pc >= 16"
	//其可行值为33~cb
	bool sdPlayer;                   // 轮到谁走，0=红方，1=黑方
	PositionStruct() {
		FromFen(cszStartFen);
	}
	PositionStruct(const char* const arr) {
		FromFen(arr);
	}
	void ClearBoard();
	void FromFen(const char* const);
	void AddPiece(int, int); // 在棋盘上放一枚棋子
	void DelPiece(int); // 从棋盘上拿走一枚棋子
};

struct ChessBoard {//棋盘
	char ucpcSquares[256];
	ChessBoard(PositionStruct&);
	void DrawBoard();
};