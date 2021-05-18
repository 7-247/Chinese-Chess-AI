#include <cstring>

#include "Evaluate.h"
#include "Position.h"
#include "Search.h"
#include "Ucci.h"
using namespace std;

int main() {
    char input[MAXLEN], tep[MAXLEN], arr[MAXLEN];  //给阿谊的串存在arr[]
    memset(tep, 0, sizeof(char) * MAXLEN);
    memcpy(
        arr,
        "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1",
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
        } /*else if (!strncmp(input, "position ", 9 * sizeof(char))) {
            memcpy(tep, input + 9, 591);
            if (!strncmp(tep, "startpos", 8 * sizeof(char))) {
                memcpy(tep,
                       "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR
        r - - 0 1", 69 * sizeof(char)); memcpy(tep + 69, input + 17, (MAXLEN -
        69) * sizeof(char)); if (judge(tep, MAXLEN)) { memcpy(arr, tep, MAXLEN);
                    mychess.PositionInit(arr);  //然后把改变后的c转化为局面
                }
            } else if (!strncmp(tep, "fen ", 4 * sizeof(char))) {
                memmove(tep, tep + 4, (MAXLEN - 4) * sizeof(char));
                if (judge(tep, MAXLEN)) {
                    memcpy(arr, tep, MAXLEN);
                    mychess.PositionInit(arr);  //然后把改变后的c转化为局面
                }
            }
        }*/
        else if (!strcmp(input, "quit")) {
            PrintLn("bye");
            break;
        } /*else if (!strncmp(input, "go time ", 8 * sizeof(char))) {
            memcpy(tep, input + 8, (MAXLEN - 8) * sizeof(char));
            if (gettime(tep, MAXLEN, gotime)) {
                // printf("set search time:%d ms\n", gotime);
                //以下应为搜索得出bestmove和走法
                int move = SearchMain(mychess, gotime);
                if (move == 0) {
                    PrintLn("error");
                    continue;
                }
                BestMoveIntToChar(move);
            }
        }*/
        else {
            mychess.PositionInit(input);  //然后把改变后的c转化为局面
            cout << "当前局面：" << endl;
            ChessBoard myboard(mychess);
            myboard.DrawBoard();

            int move = SearchMain(mychess, gotime);
            if (move == 0) {
                PrintLn("error");
                continue;
            }
            BestMoveIntToChar(move);          //用于最后的输出
            BestMoveIntToNum(mychess, move);  //用于象棋巫师调试'

            Eval myeval(mychess);
            cout << "当前评估值：" << myeval.GetEvalNum() << endl;
            cout << "Red:" << myeval.EvalRed << " "
                 << "Black:" << myeval.EvalBlack << endl;

            /*vector<vector<int>> tep = mychess.Predict();
            for (int i = 0; i < 32; i++) {
                if (!mychess.ucsqPieces[i]) continue;
                if (i >= 16)
                    cout << cszPieceBytesInChineseBlack[cnPieceTypes[i]]
                         << ":\n";
                else
                    cout << cszPieceBytesInChineseRed[cnPieceTypes[i]] << ":\n";
                PosIntToChar(mychess.ucsqPieces[i]);
                cout << endl;
                for (int j = 0; j < tep[i].size(); j++) {
                    cout << " ";
                    PosIntToChar(tep[i][j]);
                }
                cout << endl;
            }*/
        }
    }
    return 0;
}
