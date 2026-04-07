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

:loop
cls
echo [1/2] Compilando objeto: %nome%.o...
gcc -c -o "%nome%.o" "%arquivo%" -I"C:\MinGW\include"

if %errorlevel% neq 0 goto erro_comp

echo [2/2] Linkando bibliotecas: %nome%.exe...
gcc -o "%nome%.exe" "%nome%.o" -L"C:\MinGW\lib" -lfreeglut -lopengl32 -lglu32 -lwinmm

if %errorlevel% neq 0 goto erro_link

echo.
echo =======================================
echo    Sucesso! Abrindo %nome%.exe...
echo =======================================
echo.
start "" "%nome%.exe"

:pergunta
echo.
set /p opcao="Deseja recompilar as alteracoes? (S para Sim, N para Fechar Tudo): "
if /i "%opcao%"=="S" (
    :: Força o fechamento do executável antigo silenciosamente
    taskkill /F /IM "%nome%.exe" >nul 2>&1
    goto loop
)
if /i "%opcao%"=="N" (
    taskkill /F /IM "%nome%.exe" >nul 2>&1
    exit
)
:: Se digitar qualquer outra coisa, pergunta de novo
goto pergunta

:erro_comp
echo.
echo [ERRO] Falha na compilacao do objeto.
pause
goto pergunta

:erro_link
echo.
echo [ERRO] Falha ao criar o executavel final.
pause
goto pergunta