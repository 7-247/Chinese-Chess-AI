#include "ChessBoard.h"


inline void PrintLn(const char* sz) {
	printf("%s\n", sz);
	fflush(stdout);
}
int main() {
	char arr[600]="rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
	PositionStruct mychess(arr);
	ChessBoard myboard(mychess);
	myboard.DrawBoard();
	return 0;
}
