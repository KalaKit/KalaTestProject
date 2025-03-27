@echo off

echo Building debug...
echo.

cmd /c build_windows.bat cmake debug

echo.
echo Building release...
echo.

cmd /c build_windows.bat cmake release

echo.
echo Successfully built TestProject!

pause
exit /b 0