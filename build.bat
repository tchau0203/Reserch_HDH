@echo off
echo ==============================================================
echo [BUILD] Bien dich chuong trinh Mo phong Dich dia chi Bang trang
echo ==============================================================

g++ -std=c++17 -Wall -Wextra -Iinclude src\main.cpp src\page_table.cpp src\address_translation.cpp -o main.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Bien dich that bai!
    exit /b %ERRORLEVEL%
)

echo [SUCCESS] Bien dich thanh cong! File thuc thi: main.exe
echo.
echo De chay chuong trinh:
echo   main.exe                (Chay giao dien Menu tuong tac)
echo   main.exe --test         (Chay kiem thu 8 test cases mau)
echo.
