#include <cstring>
#include <random>

#include "Evaluate.h"
#include "Position.h"
#include "Search.h"
#include "Ucci.h"
using namespace std;
long long hsh[15][100];
int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);

    for (int i = 0; i < 15; ++i)
        for (int j = 1; j <= 90; ++j)
            for (int k = 1; k <= 3; ++k) hsh[i][j] = hsh[i][j] * 1000000 + dis(gen);

    char input[MAXLEN], tep[MAXLEN],
        arr[MAXLEN];  //给阿谊的串存在arr[]
    memset(tep, 0, sizeof(char) * MAXLEN);
    memcpy(arr,
           "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/"
           "RNBAKABNR r - - 0 1",
           70);
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
    while (true) {
        cin.getline(input, MAXLEN);
        if (!strncmp(input, "isready", 7 * sizeof(char))) {
            PrintLn("readyok");

        } else if (!strncmp(input, "position ", 9 * sizeof(char))) {
            memcpy(tep, input + 9, MAXLEN - 9);
            if (!strncmp(tep, "startpos", 8 * sizeof(char))) {
                memcpy(tep,
                       "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/"
                       "RNBAKABNR r - - 0 1",
                       69 * sizeof(char));
                memcpy(tep + 69, input + 17, (MAXLEN - 69) * sizeof(char));
                if (judge(tep, MAXLEN)) {
                    memcpy(arr, tep, MAXLEN);
                    mychess.PositionInit(arr);  //然后把改变后的c转化为局面
                }
            } else if (!strncmp(tep, "fen ", 4 * sizeof(char))) {
                memmove(tep, tep + 4, (MAXLEN - 4) * sizeof(char));
                if (judge(tep, MAXLEN)) {
                    memcpy(arr, tep, MAXLEN);
                    mychess.PositionInit(arr);  //然后把改变后的c转化为局面
                    // ChessBoard outp(mychess);
                    // outp.DrawBoard();
                }
            }
        } else if (!strcmp(input, "quit")) {
            PrintLn("bye");
            break;
        } else if (!strncmp(input, "go time ", 8 * sizeof(char))) {
            memcpy(tep, input + 8, (MAXLEN - 8) * sizeof(char));
            if (gettime(tep, MAXLEN, gotime)) {
                //以下应为搜索得出bestmove和走法
                int move = SearchMain(mychess, gotime);
                if (move == 0) {
                    PrintLn("no found");
                    continue;
                }
                BestMoveIntToChar(move);  //用于最后的输出

                // BestMoveIntToNum(mychess, move);  //用于象棋巫师调试

                mychess.ChangeBoard(move);  //改变局面
                ChessBoard myboard(mychess);
                myboard.DrawBoard();  //画图
            }
        }
    }
    return 0;
}
