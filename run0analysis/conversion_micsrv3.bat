@echo off

setlocal ENABLEEXTENSIONS

set BAT_DIR=%~dp0
echo %BAT_DIR%
set log_file=%BAT_DIR%\tracks.raw.root.log
set clust_dir=D:\SND\RUN0_W3_B1

pushd %clust_dir%
for /D %%I in (P008_checkangles_rotated) do (
  echo.
  echo Processing '%BAT_DIR%\%%I'
  if not exist %BAT_DIR%\%%I (
    mkdir %BAT_DIR%\%%I 1>nul 2>&1
    pushd %BAT_DIR%\%%I
    copy %clust_dir%\%%I\tracks.obx .\tracks.obx
    C:\LASSO_x64\win32\EdbConv.exe -mode:OPERA_MT .\tracks.obx
    popd
  ) else (
    echo Target directory exists '%BAT_DIR%\%%I'
    echo Please remove/rename the target directory if you want to reprocess it
    echo Processing cancelled
  )
)
popd

endlocal
