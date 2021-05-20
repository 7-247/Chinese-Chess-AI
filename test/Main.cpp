#include <cstring>
#include "pch.h"
#include "Evaluate.h"
#include "Position.h"
#include "Search.h"
#include "Ucci.h"
//#pragma warning(disable:6054)
using namespace std;
char startstr[1000] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1 moves";
char thinktime[MAXLEN] = "go time 2";//��������ҵ1msʱ��
bool wefirst;
bool showboard = false;//�Ƿ���ʾ����
int drawnum, losenum, winnum;
int main() {
	char input[MAXLEN], tep[MAXLEN], arr[MAXLEN];  //������Ĵ�����arr[]
	memset(tep, 0, sizeof(tep));
	memcpy(arr, startstr, sizeof(startstr));
	int gotime = 0;  //���ҳ���ʱ�����int k��

	//��ʼһ��Ϊucci�����򲻸���
	PositionStruct mychess;
	while (true) {
		cin.getline(input, MAXLEN);
		if (!strcmp(input, "ucci")) {
			PrintLn("ucciok");
			break;
		}
	}
	// aΪ���봮��bΪ��������еĴ���cΪ������
	// c������judge����trueʱ�ſɸı䣬��Ϊ�ⲿָ������
	for (int u = 1; u <= 20; ++u) {
		memset(input, 0, sizeof(input));
		memcpy(input, "position fen rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1 moves", 89);

		mychess.sdPlayer = 0;
		mychess.PositionInit(startstr);
		wefirst = !wefirst;//wefirstΪ1��Ϊ��������
		if (wefirst) {
			printf("�ҷ�����\n");
			int move = SearchMain(mychess, gotime);
			int iseat = PosToNo(move & 255, mychess.ucsqPieces);//û�����򷵻�-1
			mychess.ChangeBoard(move);  //�ı����
			mychess.sdPlayer = !mychess.sdPlayer;//�ֻ�
			ChessBoard myboard(mychess);
			if (iseat != -1)myboard.BoardToFen(input);//���ӣ��ı�fen��
			else myboard.BoardToFenMoves(input, move);//�����ӣ������
			if(showboard)myboard.DrawBoard();
		}
		else { printf("�Է�����\n"); }
		while (true) {
			//cin.getline(input, MAXLEN);
			{
				int otherstate = mychess.Repeat();
				if (otherstate == 0 || mychess.Count >= 120) {
					++drawnum;
					printf("%d:draw\n", u);
					ChessBoard myboard(mychess);
					myboard.DrawBoard();
					break;
				}
			}

			{
				//printf("-----------------------------------\n�Է��غ�\n");
				memcpy(tep, input, sizeof(input));
				int k = mydll(input, thinktime);
				if (k == 1012808798) {//����Ӯ��
					++winnum;
					cout <<"�������ұ������ˣ�Ҳ����������ҵ�ʱ�䲻��������ҵĴ�Ϊ:\n"<< tep << endl;
					printf("%d:win\n", u);
					ChessBoard myboard(mychess);
					myboard.DrawBoard();
					break;
				}
				memcpy(input, tep, sizeof(tep));
				int formatmove = ChangeToFormatMove(k);   //���Ǵ���ƶ�������Ϊint -7453
				int iseat = PosToNo(formatmove & 255, mychess.ucsqPieces);//û�����򷵻�-1
				mychess.ChangeBoard(formatmove);  //�ı����
				mychess.sdPlayer = !mychess.sdPlayer;//�ֻ�
				ChessBoard myboard(mychess);
				if (iseat != -1)myboard.BoardToFen(input);//���ӣ��ı�fen��
				else myboard.BoardToFenMoves(input, formatmove);//�����ӣ������
				if (showboard)myboard.DrawBoard();
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
						mychess.PositionInit(arr);  //Ȼ��Ѹı���cת��Ϊ����
					}
				}
				else if (!strncmp(tep, "fen ", 4 * sizeof(char))) {
					memmove(tep, tep + 4, (MAXLEN - 4) * sizeof(char));
					if (judge(tep, MAXLEN)) {
						memcpy(arr, tep, MAXLEN);
						mychess.PositionInit(arr);  //Ȼ��Ѹı���cת��Ϊ����
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
					//����ӦΪ�����ó�bestmove���߷�
					//int move = SearchMain(mychess, gotime);
					//BestMoveIntToChar(move);
				}
			}
			/*else */
			{
				int otherstate = mychess.Repeat();
				if (otherstate == 0 || mychess.Count >= 120) {
					++drawnum;
					printf("%d:draw\n", u);
					ChessBoard myboard(mychess);
					myboard.DrawBoard();
					break;
				}
			}
			{
				//printf("-----------------------------------\n�ҵĻغ�\n");
				int move = SearchMain(mychess, gotime);
				{
					if ((move == -1 && wefirst) || (move == 1 && !wefirst)||move==0) {
						++losenum;
						printf("%d:lose\n", u);
						ChessBoard myboard(mychess);
						myboard.DrawBoard();
						break;
					}
					else if ((move == -1 && !wefirst) || (move == 1 && wefirst)) {
						++winnum;
						printf("%d:win\n", u);
						ChessBoard myboard(mychess);
						myboard.DrawBoard();
						break;
					}
				}
				//BestMoveIntToChar(move);          //�����������
				int iseat = PosToNo(move & 255, mychess.ucsqPieces);//û�����򷵻�-1
				mychess.ChangeBoard(move);  //�ı����
				mychess.sdPlayer = !mychess.sdPlayer;//�ֻ�
				ChessBoard myboard(mychess);
				if (iseat != -1)myboard.BoardToFen(input);//���ӣ��ı�fen��
				else myboard.BoardToFenMoves(input, move);//�����ӣ������
				if (showboard)myboard.DrawBoard();
				//BestMoveIntToNum(mychess, move);  //����������ʦ����
			}
		}
	}
	printf("win:%d\ndraw:%d\nlose:%d\n", winnum, drawnum, losenum);
	return 0;
}
