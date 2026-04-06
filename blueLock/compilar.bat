@echo off
setlocal enabledelayedexpansion
cls
title Compilador Universal OpenGL

:: Verifica se algum arquivo foi fornecido
if "%~1"=="" (
    echo [AVISO] Arraste um arquivo .c para cima deste script ou 
    set /p arquivo="Digite o nome do arquivo (com .c): "
) else (
    set arquivo=%~1
)

:: Extrai o nome sem a extensao para usar no .o e .exe
set nome=%~n1
if "%nome%"=="" set nome=programa

echo [1/2] Compilando objeto: %nome%.o...
gcc -c -o "%nome%.o" "%arquivo%" -I"C:\MinGW\include"

if %errorlevel% neq 0 (
    echo.
    echo [ERRO] Falha na compilacao do objeto.
    pause
    exit /b
)

echo [2/2] Linkando bibliotecas: %nome%.exe...
gcc -o "%nome%.exe" "%nome%.o" -L"C:\MinGW\lib" -lfreeglut -lopengl32 -lglu32

if %errorlevel% equ 0 (
    echo.
    echo =======================================
    echo    Sucesso! Abrindo %nome%.exe...
    echo =======================================
    echo.
    start "" "%nome%.exe"
) else (
    echo.
    echo [ERRO] Falha ao criar o executavel final.
)

pause