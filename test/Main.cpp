#include <cstring>
#include "pch.h"
#include "Evaluate.h"
#include "Position.h"
#include "Search.h"
#include "Ucci.h"
//#pragma warning(disable:6054)
using namespace std;
char startstr[100] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1 moves";
char thinktime[MAXLEN] = "go time 2";//给优秀作业10ms时间
bool wefirst = 1;
int drawnum, losenum, winnum, roundnum;
int main() {
	char input[MAXLEN], tep[MAXLEN], arr[MAXLEN];  //给阿谊的串存在arr[]
	memset(tep, 0, sizeof(tep));
	memcpy(arr, startstr, sizeof(startstr));
	int gotime = 0;  //给室长的时间存在int k里

	//开始一定为ucci，否则不给开
	PositionStruct mychess;
	while (true) {
		cin.getline(input, MAXLEN);
		if (!strcmp(input, "ucci")) {
			PrintLn("ucciok");
			break;
		}
	}
	// a为输入串，b为处理过程中的串，c为处理结果
	// c仅有在judge返回true时才可改变，即为外部指定局面
	for (int u = 1; u <= 10; ++u) {
		memset(input, 0, sizeof(input));
		memcpy(input, "position fen rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1 moves", 89);
		mychess.sdPlayer = 0;
		roundnum = 0;
		mychess.PositionInit(startstr);
		wefirst = !wefirst;//wefirst为1即为我们先手
		if (wefirst) {
			printf("我方先手\n");
			int move = SearchMain(mychess, gotime);
			int iseat = PosToNo(move & 255, mychess.ucsqPieces);//没吃子则返回-1
			mychess.ChangeBoard(move);  //改变局面
			mychess.sdPlayer = !mychess.sdPlayer;//轮换
			ChessBoard myboard(mychess);
			if (iseat != -1)myboard.BoardToFen(input);//吃子，改变fen串
			else myboard.BoardToFenMoves(input, move);//不吃子，往后加
			//myboard.DrawBoard();
		}
		else { printf("对方先手\n"); }
		while (true) {
			//cin.getline(input, MAXLEN);

			{
				int otherstate = mychess.Repeat();
				if (otherstate == 0 || roundnum >= 120) {
					++drawnum;
					printf("%d:draw\n", u);
					ChessBoard myboard(mychess);
					myboard.DrawBoard();
					break;
				}
			}

			{
				//printf("-----------------------------------\n对方回合\n");
				memcpy(tep, input, sizeof(input));
				//cout << input << endl;
				int k = mydll(input, thinktime);
				if (k == 1012808798) {//我们赢了
					++winnum;
					printf("%d:win\n", u);
					ChessBoard myboard(mychess);
					myboard.DrawBoard();
					break;
				}
				memcpy(input, tep, sizeof(tep));
				//printf("优秀作业的走法:%.4s\n", (const char*)&k);
				int formatmove = ChangeToFormatMove(k);   //我们存放移动的数据为int -7453
				//cout << formatmove<<endl;
				int iseat = PosToNo(formatmove & 255, mychess.ucsqPieces);//没吃子则返回-1
				mychess.ChangeBoard(formatmove);  //改变局面
				mychess.sdPlayer = !mychess.sdPlayer;//轮换
				ChessBoard myboard(mychess);
				if (iseat != -1)myboard.BoardToFen(input);//吃子，改变fen串
				else myboard.BoardToFenMoves(input, formatmove);//不吃子，往后加
				++roundnum;
				//myboard.DrawBoard();
				//cout << input << endl;
			}
			if (!strncmp(input, "isready", 7 * sizeof(char))) {
				PrintLn("readyok");
			}
			else if (!strncmp(input, "position ", 9 * sizeof(char))) {
				memcpy(tep, input + 9, MAXLEN - 9);
				if (!strncmp(tep, "startpos", 8 * sizeof(char))) {
					memcpy(tep, "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1", 69 * sizeof(char));
					memcpy(tep + 69, input + 17, (MAXLEN - 69) * sizeof(char));
					if (judge(tep, MAXLEN)) {
						memcpy(arr, tep, MAXLEN);
						mychess.PositionInit(arr);  //然后把改变后的c转化为局面
					}
				}
				else if (!strncmp(tep, "fen ", 4 * sizeof(char))) {
					memmove(tep, tep + 4, (MAXLEN - 4) * sizeof(char));
					if (judge(tep, MAXLEN)) {
						memcpy(arr, tep, MAXLEN);
						mychess.PositionInit(arr);  //然后把改变后的c转化为局面
					}
				}
			}
			else if (!strcmp(input, "quit")) {
				PrintLn("bye");
				break;
			}
			else if (!strncmp(input, "go time ", 8 * sizeof(char))) {
				memcpy(tep, input + 8, (MAXLEN - 8) * sizeof(char));
				if (gettime(tep, MAXLEN, gotime)) {
					// printf("set search time:%d ms\n", gotime);
					//以下应为搜索得出bestmove和走法
					//int move = SearchMain(mychess, gotime);
					//BestMoveIntToChar(move);
				}
			}
			/*else */
			{
				int otherstate = mychess.Repeat();
				if (otherstate == 0 || roundnum >= 120) {
					++drawnum;
					printf("%d:draw\n", u);
					ChessBoard myboard(mychess);
					myboard.DrawBoard();
					break;
				}
			}
			{
				//printf("-----------------------------------\n我的回合\n");
				int move = SearchMain(mychess, gotime);
				{
					if ((move == -1 && wefirst) || (move == 1 && !wefirst)) {
						++losenum;
						printf("%d:lose\n", u);
						ChessBoard myboard(mychess);
						myboard.DrawBoard();
						break;
					}
				}
				//BestMoveIntToChar(move);          //用于最后的输出
				int iseat = PosToNo(move & 255, mychess.ucsqPieces);//没吃子则返回-1
				mychess.ChangeBoard(move);  //改变局面
				mychess.sdPlayer = !mychess.sdPlayer;//轮换
				ChessBoard myboard(mychess);
				if (iseat != -1)myboard.BoardToFen(input);//吃子，改变fen串
				else myboard.BoardToFenMoves(input, move);//不吃子，往后加
				//myboard.DrawBoard();
				//cout << input << endl;
				//BestMoveIntToNum(mychess, move);  //用于象棋巫师调试
			}
		}
	}
	printf("win:%d\ndraw:%d\nlose:%d\n", winnum, drawnum, losenum);
	return 0;
}
