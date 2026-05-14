@echo off
chcp 65001 >nul
setlocal

set PYTHON=python
set SCRIPT=RGB2HSV.py
set DEPS=PyOpenGL PyOpenGL_accelerate Pillow numpy pyopengltk

if "%1"==""        goto all
if "%1"=="install" goto install
if "%1"=="run"     goto run
if "%1"=="clean"   goto clean
if "%1"=="help"    goto help

echo Comando desconhecido: %1
goto help

:all
call :install
call :run
goto end

:install
echo.
echo [1/2] Instalando dependencias...
%PYTHON% -m pip install --upgrade %DEPS%
if errorlevel 1 (
    echo ERRO: Falha ao instalar dependencias.
    exit /b 1
)
echo Dependencias instaladas com sucesso.
goto :eof

:run
echo.
echo [2/2] Executando %SCRIPT%...
%PYTHON% %SCRIPT%
if errorlevel 1 (
    echo ERRO: O programa encerrou com falha.
    exit /b 1
)
goto :eof

:clean
echo.
echo Removendo cache Python...
for /d /r . %%d in (__pycache__) do (
    if exist "%%d" rd /s /q "%%d"
)
del /s /q *.pyc 2>nul
echo Cache limpo.
goto end

:help
echo.
echo  Uso:  run.bat [comando]
echo.
echo  Comandos disponiveis:
echo    (nenhum)   instala dependencias e executa o programa
echo    install    instala apenas as dependencias
echo    run        executa o programa
echo    clean      remove cache Python (__pycache__ e .pyc)
echo    help       exibe esta ajuda
echo.

:end
endlocal
