@echo off
title 天威诚信自动批量代码签名工具
rem   =========检测系统是否安装 Signtool=============
if not exist tool\signtool.exe goto NoExistSigntool
if not exist tool\capicom.dll goto NoExistSigntool

rem  =========检查签名工具是否安装=========

if exist "%SystemRoot%\system32\capicom.dll" del "%SystemRoot%\system32\capicom.dll" /q /s > nul
copy tool\capicom.dll "%SystemRoot%\system32\capicom.dll" /y > nul
regsvr32 "%SystemRoot%\system32\capicom.dll" /s


rem  =========生成待签名文件列表=========

:listfile
rem  =========检查是否存已存在待签名文件列表，如存在则删除=========
if exist list.txt del list.txt /q /s
cls
color 0f
echo.
echo                     ╭─────────────────╮
echo         ╭─────┤   天威诚信自动批量代码签名工具   ├─────╮
echo         │          ╰─────────────────╯          │
echo         │    操作说明：                                            │
echo         │                                                          │
echo         │        本程序应用微软signtool工具，可批量或为单个代码    │
echo         │                                                          │
echo         │    添加签名。                                            │
echo         │                                                          │
echo         │        签名文件列表list.txt                              │
echo         │                                                          │
echo         │                  技术支持请联系：www.itrus.com.cn        │
echo         │                                                          │
echo         ╰─────────────────────────────╯
echo.
echo          任意键继续...
pause >nul

cls
echo.
echo.
echo         ====提示：可直接拖拽 pfx 文件至窗口中，pfx 文件不能为空口令====
echo.
echo         请输入代码签名证书pfx文件路径
set pfxfilesource=:
echo.
set /p pfxfilesource=         pfx文件路径：
set "pfxfilesource=%pfxfilesource:"=%"
:: 上面这句为判断%pfxfilesource%中是否存在引号，有则剔除。
if not exist "%pfxfilesource%" goto nopfxfile

:setpfxpasswd
echo.
set pfxpasswd=:
set /p pfxpasswd=         请输入pfx文件密码：
if "%pfxpasswd%"==":" goto setpfxpasswd

echo.
echo.
echo         ====提示：可直接拖拽待签名 程序 或 程序所在文件夹 至窗口中====
echo.
echo          请输入签名文件夹或单个文件

:setinput
echo.
set input=:
set /p input=         文件路径：
set "input=%input:"=%"
:: 上面这句为判断%input%中是否存在引号，有则剔除。

if "%input%"==":" goto setinput
if not exist "%input%" goto errorpath

pushd "%input%">nul 2>nul && (popd & set folder=yes)||(set folder=no)

:: 上面这句判断用户输入的是一个文件夹还是一个文件

goto multifile%folder%

rem 如果输入的是一个文件

:multifileno
cls
echo.
echo.
Echo          ----------------------------------------------------------
Echo                          单个代码添加签名模式开始
Echo          ----------------------------------------------------------
echo.

echo.
echo         ╔═════════════════════════════╗
echo         ║                                                          ║
echo                           输入软件描述信息（可选）
echo         ║                                                          ║
set describe=:
set /p describe=             软件描述：
echo         ║                                                          ║
set describeURL=:
set /p describeURL=             描述链接(包含http/https)：
echo         ║                                                          ║
echo         ╚═════════════════════════════╝
echo.

if "%describe%"==":" goto s_no_describe
if "%describeURL%"==":" goto s_no_describe

tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /d "%describe%" /du "%describeURL%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%input%"
goto choice1

:s_no_describe
tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%input%"
echo %input%>list.txt
ping 127.1 -n 4 >nul

:choice1
echo.
echo              ╔═══════════════════════╗
echo              ║                                              ║
echo              ║            单个代码添加签名完成              ║
echo              ║                                              ║
echo              ║       请选择 0 退出 / 1 返回继续签名。       ║
echo              ║                                              ║
echo              ╚═══════════════════════╝
echo.
set choice1=
set /p choice1=                           请选择(0/1): 
if /i "%choice1%"=="0" goto eof
if /i "%choice1%"=="1" goto listfile
echo          错误的选择
goto choice1

rem 如果输入的是一个文件夹

:multifileyes
cls

for /r "%input%" %%i in (*.exe *.dll *.cab *.ocx *.mui *.vbs *.sys *.cat *.msi) do echo %%~fi>>list.txt
if not exist list.txt goto no_suitfile

echo.
echo.
Echo          ----------------------------------------------------------
Echo                          批量代码添加签名模式开始
Echo          ----------------------------------------------------------
echo.

echo.
echo         ╔═════════════════════════════╗
echo         ║                                                          ║
echo                           输入软件描述信息（可选）
echo         ║                                                          ║
set describe=:
set /p describe=             软件描述：
echo         ║                                                          ║
set describeURL=:
set /p describeURL=             描述链接(包含http/https)：
echo         ║                                                          ║
echo         ╚═════════════════════════════╝
echo.

if "%describe%"==":" goto m_no_describe
if "%describeURL%"==":" goto m_no_describe

for /f "delims=" %%1 in (list.txt) do tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /d "%describe%" /du "%describeURL%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%%~1"
goto choice2

:m_no_describe
for /f "delims=" %%1 in (list.txt) do tool\signtool.exe sign /f "%pfxfilesource%" /p "%pfxpasswd%" /t "http://timestamp.verisign.com/scripts/timstamp.dll" "%%~1"
ping 127.1 -n 4 >nul

:choice2
echo.
echo              ╔═══════════════════════╗
echo              ║                                              ║
echo              ║            批量代码添加签名完成              ║
echo              ║                                              ║
echo              ║       请选择 0 退出 / 1 返回继续签名。       ║
echo              ║                                              ║
echo              ╚═══════════════════════╝
echo.
set choice2=
set /p choice2=                           请选择(0/1): 
if /i "%choice2%"=="0" goto eof
if /i "%choice2%"=="1" goto listfile
echo          错误的选择
goto choice2



echo      已完成程序签名操作。程序将打开已签名的文件列表并自动退出。
ping 127.1 -n 6 >nul
start list.txt
ping 127.1 -n 4 >nul
goto exit


:errorpath
cls
echo.
echo.
echo.
echo                     ╭─────────────────╮
echo         ╭─────┤   天威诚信自动批量代码签名工具   ├─────╮
echo         │          ╰─────────────────╯          │
echo         │                                                          │
echo         │    目录或文件不存在，请重新输入！                        │
echo         │                                                          │
echo         │                             按任意键返回...              │
echo         │                                                          │
echo         ╰─────────────────────────────╯
pause > nul
goto listfile

:no_suitfile
cls
echo.
echo.
echo.
echo                     ╭─────────────────╮
echo         ╭─────┤   天威诚信自动批量代码签名工具   ├─────╮
echo         │          ╰─────────────────╯          │
echo         │                                                          │
echo                "%input%"
echo         │                                                          │
echo         │    目录下没有可签名文件，请重新输入目录！                │
echo         │                                                          │
echo         │                             按任意键返回...              │
echo         │                                                          │
echo         ╰─────────────────────────────╯
pause > nul
goto listfile

:NoExistSigntool
cls
color 0c
echo.
echo.
echo.
echo.
echo.
echo                             ╭──────────╮
echo           ╭────────┤  Signtool 安装错误 ├────────╮
echo           │                ╰──────────╯                │
echo           │                                                        │
echo           │       Signtool 缺失，请检查程序完整性后重试            │
echo           │                                                        │
echo           ╰────────────────────────────╯
ping 127.1 -n 6 >nul
echo.
echo           程序即将退出
start http://www.itrus.com.cn/
ping 127.1 -n 4 >nul && goto :eof