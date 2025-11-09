@echo off
REM ===============================
REM Build, upload firmware, upload SPIFFS, dan buka Serial Monitor ESP32
REM ===============================

REM Path PlatformIO CLI
set PIO_PATH=C:\Users\Heria\.platformio\penv\Scripts\pio.exe

echo ===============================
echo BUILDING PROJECT...
echo ===============================
"%PIO_PATH%" run
if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo ===============================
echo UPLOADING FIRMWARE (CODE)...
echo ===============================
"%PIO_PATH%" run --target upload --upload-port COM5
if errorlevel 1 (
    echo Upload firmware failed!
    pause
    exit /b 1
)

echo ===============================
echo UPLOADING FILESYSTEM (WEB FILES)...
echo ===============================
"%PIO_PATH%" run --target uploadfs
if errorlevel 1 (
    echo Upload SPIFFS failed!
    pause
    exit /b 1
)

echo ===============================
echo OPENING SERIAL MONITOR...
echo ===============================
REM Gunakan & di PowerShell, atau jalankan di CMD langsung
"%PIO_PATH%" device monitor --baud 115200 --port COM5

echo ===============================
echo --- DONE ---
echo ===============================
pause
