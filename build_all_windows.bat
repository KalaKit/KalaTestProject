@echo off

set "PROJECT_ROOT=%~dp0"
cd "%PROJECT_ROOT%"

echo =====================================
echo [INFO] Copying external files...
echo =====================================
echo.

cmd /c "_external_shared\copy_windows.bat"
if errorlevel 1 (
    echo [ERROR] Copy failed.
    pause
    exit /b 1
)

echo =====================================
echo [INFO] Building test project in Release mode...
echo =====================================
echo.

cmd /c "build_windows_release.bat"
if errorlevel 1 (
    echo [ERROR] Release build failed.
    pause
    exit /b 1
)

echo.
echo =====================================
echo [INFO] Building test project in Debug mode...
echo =====================================
echo.

cmd /c "build_windows_debug.bat"
if errorlevel 1 (
    echo [ERROR] Debug build failed.
    pause
    exit /b 1
)

echo.
echo =====================================
echo [SUCCESS] Finished building and installing test project!
echo =====================================
echo.

pause
exit /b 0
