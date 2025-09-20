@echo off
echo QuadTree C++ Build Script
echo ========================

REM Try different compilers
echo Checking for available compilers...

REM Check for g++
where g++ >nul 2>&1
if %errorlevel% == 0 (
    echo Found g++, compiling...
    g++ -std=c++14 -Wall -Wextra -O2 -o QuadTreeExample.exe QuadTree.cpp QuadTreeExample.cpp
    if %errorlevel% == 0 (
        echo Build successful with g++!
        echo Run with: QuadTreeExample.exe
        goto :end
    )
)

REM Check for clang++
where clang++ >nul 2>&1
if %errorlevel% == 0 (
    echo Found clang++, compiling...
    clang++ -std=c++14 -Wall -Wextra -O2 -o QuadTreeExample.exe QuadTree.cpp QuadTreeExample.cpp
    if %errorlevel% == 0 (
        echo Build successful with clang++!
        echo Run with: QuadTreeExample.exe
        goto :end
    )
)

REM Check for Visual Studio cl
where cl >nul 2>&1
if %errorlevel% == 0 (
    echo Found Visual Studio cl, compiling...
    cl /EHsc /std:c++14 /O2 QuadTree.cpp QuadTreeExample.cpp /Fe:QuadTreeExample.exe
    if %errorlevel% == 0 (
        echo Build successful with Visual Studio!
        echo Run with: QuadTreeExample.exe
        goto :end
    )
)

echo No suitable C++ compiler found!
echo Please install one of the following:
echo - MinGW-w64 (provides g++)
echo - LLVM/Clang
echo - Visual Studio Build Tools
echo.
echo Or compile manually with your preferred compiler:
echo   compiler -std=c++14 -O2 -o QuadTreeExample QuadTree.cpp QuadTreeExample.cpp

:end
pause