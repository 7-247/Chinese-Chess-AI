#include <cstring>
#include "pch.h"
#include "Evaluate.h"
#include "Position.h"
#include "Search.h"
#include "Ucci.h"
//#pragma warning(disable:6054)
using namespace std;
char positionfenarr[MAXLEN]="position fen rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
char startstr[100] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
char thinktime[MAXLEN] = "go time 2";//��������ҵ10msʱ��

int drawnum, losenum, winnum,roundnum;
int main() {
    char input[MAXLEN], tep[MAXLEN], arr[MAXLEN];  //������Ĵ�����arr[]
    memset(tep, 0, sizeof(char) * MAXLEN);
    memcpy(arr,startstr,sizeof(startstr));
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
    for(int u=1;u<=100;++u) {
        memcpy(positionfenarr, "position fen rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1",MAXLEN);
        roundnum = 0;
        mychess.PositionInit(startstr);
        while (true) {
            //cin.getline(input, MAXLEN);
            //printf("-----------------------------------\n�Է��غ�\n");
            int k = mydll(positionfenarr, thinktime);
            //printf("������ҵ���߷�:%.4s\n", (const char*)&k);
            int formatmove = ChangeToFormatMove(k);   //���Ǵ���ƶ�������Ϊint
            mychess.sdPlayer = !mychess.sdPlayer;//�ֻ�Ϊ�ڷ�
            mychess.ChangeBoard(formatmove);  //�ı����
            ChessBoard myboard(mychess);
            //myboard.DrawBoard();
            myboard.BoardToFen(input);//�ı�fen��
            //cout << input << endl;
            ++roundnum;

            if (!strncmp(input, "isready", 7 * sizeof(char))) {
                PrintLn("readyok");
            }
            else if (!strncmp(input, "position ", 9 * sizeof(char))) {
                memcpy(tep, input + 9, 591);
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
                //printf("-----------------------------------\n�ҵĻغ�\n");
                int move = SearchMain(mychess, gotime,roundnum);
                if (move == 0) {
                    ++drawnum;
                    printf("%d:draw\n", u);
                    break;
                }
                else if (move == -1) {
                    ++losenum;
                    printf("%d:lose\n", u);
                    break;
                }
                else if (move == 1) {
                    ++winnum;
                    printf("%d:win\n", u);
                    break;
                }
                //BestMoveIntToChar(move);          //�����������
                mychess.ChangeBoard(move);  //�ı����
                mychess.sdPlayer = !mychess.sdPlayer;//�ֻ�Ϊ�췽
                ChessBoard myboard(mychess);
                //myboard.DrawBoard();
                myboard.BoardToFen(positionfenarr);//�ı�fen��

                //BestMoveIntToNum(mychess, move);  //����������ʦ����
            }
        }
    }
    printf("win:%d\ndraw:%d\nlose:%d\n", winnum, drawnum, losenum);
    return 0;
}
