/*main*/
#include<iostream>
#include<cstdio>
#include"pch.h"
using namespace std;
char a[8192] = "position startpos w - - 0 17";
char b[8192] = "go time 555";

int main()
{
    //cin >> a;
    int k=mydll(a,b);
    printf("%4s", (const char*)&k);
    return 0;
}