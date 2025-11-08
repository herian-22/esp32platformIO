@echo off
set PIO_PATH=C:\Users\Heria\.platformio\penv\Scripts\pio.exe

echo Building project...
"%PIO_PATH%" run

echo UPLOADING FIRMWARE (CODE) FIRST...
"%PIO_PATH%" run --target upload --upload-port COM5

echo UPLOADING FILESYSTEM (WEBSITE) SECOND...
"%PIO_PATH%" run --target uploadfs

echo --- DONE ---
pause