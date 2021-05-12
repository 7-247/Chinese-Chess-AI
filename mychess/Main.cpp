#include "Evaluate.h"
inline void PrintLn(const char* sz) {
   printf("%s\n", sz);
   fflush(stdout);
}
#include <iostream>
int main() {
    char arr[1000];
    std::cin.getline(arr,1000);


   PositionStruct mychess(arr);
   ChessBoard myboard(mychess);
   myboard.DrawBoard();
   mychess.Generate();
   Eval myeval(mychess);
   printf("%d", myeval.GetEvalNum());
   return 0;
}
