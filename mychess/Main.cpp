#include <cstring>
#include <iostream>

#include "Evaluate.h"
using namespace std;
const int len = 1000;
const int spacenum = 5;
inline void PrintLn(const char* sz) {
    printf("%s\n", sz);
    fflush(stdout);
}
bool judge(const char* arr, int n) {
    int num = 0;
    for (int i = 0; i < n && arr[i] && num < spacenum; i++) {
        num += arr[i] == ' ';
    }
    return num == 5;
}
bool gettime(const char* arr, int n, int& time) {
    time = 0;
    for (int i = 0; i < n && arr[i]; i++) {
        if (arr[i] >= '0' && arr[i] <= '9') {
            time = time * 10 + int(arr[i] - '0');
        } else
            return false;
    }
    return true;
}
int main() {
    char input[len], tep[len], arr[len];  //给阿谊的串存在arr
    memset(tep, 0, sizeof(char) * len);
    memcpy(
        arr,
        "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1",
        70);
    int k = 0;  //给室长的时间存在k
    //开始一定为ucci，否则不给开（
    while (true) {
        cin.getline(input, len);
        if (!strcmp(input, "ucci")) {
            cout << "ucciok\n";
            fflush(stdout);
            break;
        }
    }
    // a为输入串，b为处理过程中的串，c为处理结果
    // c仅有在judge返回true时才可改变，即为外部指定局面
    while (true) {
        cin.getline(input, len);
        if (!strncmp(input, "isready", 7 * sizeof(char))) {
            printf("readyok\n");
            fflush(stdout);
        } else if (!strncmp(input, "position ", 9 * sizeof(char))) {
            memcpy(tep, input + 9, 591);
            if (!strncmp(tep, "startpos", 8 * sizeof(char))) {
                memcpy(tep,
                       "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/"
                       "RNBAKABNR r - - 0 1",
                       69 * sizeof(char));
                memcpy(tep + 69, input + 17, (len - 69) * sizeof(char));
                if (judge(tep, len)) {
                    memcpy(arr, tep, len);
                    PositionStruct mychess(arr);
                    ChessBoard myboard(mychess);
                    // myboard.DrawBoard();  //输出棋盘
                    mychess.Generate();
                    Eval myeval(mychess);
                    // printf("%d", myeval.GetEvalNum());  //输出评估值
                    //然后把改变后的c转化为局面
                }
            } else if (!strncmp(tep, "fen ", 4 * sizeof(char))) {
                memmove(tep, tep + 4, (len - 4) * sizeof(char));
                if (judge(tep, len)) {
                    memcpy(arr, tep, len);
                    PositionStruct mychess(arr);
                    ChessBoard myboard(mychess);
                    // myboard.DrawBoard();  //输出棋盘
                    mychess.Generate();
                    Eval myeval(mychess);
                    // printf("%d", myeval.GetEvalNum());  //输出评估值
                    //然后把改变后的c转化为局面
                }
            }
        } else if (!strcmp(input, "quit")) {
            printf("bye\n");
            fflush(stdout);
            break;
        } else if (!strncmp(input, "go time ", 8 * sizeof(char))) {
            memcpy(tep, input + 8, (len - 8) * sizeof(char));
            if (gettime(tep, len, k)) {
                printf("set search time:%d ms\n", k);
                fflush(stdout);
                printf("searching...\n");
                fflush(stdout);
                //以下应为搜索得出bestmove和起居
            }
        }
    }
    return 0;
}
