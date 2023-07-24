@echo off

echo ---------------------- Starting quality check ----------------------


if "%~1"=="" (
    echo No parameters specified. USAGE: check_quality.bat brick_number plate_number
    exit /B -1
)

Set Initialdir=%cd%
Set Dir=D:\RUN1_W5_B%1\P%2
Set NasDir=F:\RUN1_W5_B%1\P%2
Set PavDir=C:\LASSO_x64
Set LogDir=PavicomLog


if not exist %Dir%\ (
    echo %Dir% not found
    exit /B -1
)

echo Entering %Dir%
cd /D %Dir%
echo pwd: %cd%

if not exist %Dir%\%LogDir%\ (
    echo Copying log files
    mkdir %Dir%\%LogDir%\
    xcopy %PavDir%\!OpTraProc.log %Dir%\%LogDir%\
    xcopy %PavDir%\!PAVCameraModule.log %Dir%\%LogDir%\
    xcopy %PavDir%\!PAVGuide.log %Dir%\%LogDir%\
    xcopy %PavDir%\!PAVProcModule.log %Dir%\%LogDir%\
    xcopy %PavDir%\!PAVPointRes.log %Dir%\%LogDir%\
    xcopy %PavDir%\aaa.txt %Dir%\%LogDir%\
    xcopy %PavDir%\PAVICOM.cfg %Dir%\%LogDir%\
)
echo.
echo ---------------------- ORA SI PUO' APRIRE PAVICOM PER MUOVERE LO STAGE ----------------------
echo.

if not exist tracks.obx (
    echo tracks.obx not found. 
    exit /B -1
)

if not exist tracks.raw.root (
    echo.
    echo ---------------------- Converting .obx to .raw.root ----------------------
    C:\LASSO_X64\win32\EdbConv.exe -mode:OPERA_MT tracks.obx
)

if not exist cz.png (
    echo. 
    echo ---------------------- Doing quality plots ----------------------
    root -q -b tracks.raw.root check_raw.C
)

if not exist thickness.png (
    echo.
    echo ---------------------- Checking thickness ----------------------
    root -q -l tracks.raw.root thickness.C 
)



echo ---------------------- Backing up on NAS ----------------------

if exist %NasDir%\ (
    echo This folder already exist on the NAS
) else (
    echo Creating folder on NAS
    mkdir %NasDir%\
)

xcopy %Dir%\ %NasDir%\ /D
    
cd /D %Initialdir%

echo Done

