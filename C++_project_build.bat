setlocal enabledelayedexpansion
for %%i in (*.cpp) do set var1=!var1! %%i
echo !var1! 
g++ !var1! -o main -Wall -O2 -std=c++11 && start cmd /c "chcp 936 & cls & main & echo. & pause"