@echo off
echo.
echo ***********************************************************
echo           Message Passing Communication, v1.0 
echo ***********************************************************
echo.

@echo off
start "Client 1" Debug\Client1.exe localhost:9085 /f localhost:9080@send\demo.txt localhost:9081@send\big1.jpg 
start "Client 2" Debug\Client2.exe localhost:9086 /f localhost:9081@send\big2.jpg localhost:9080@send\1024.txt 
start "Server 1" Debug\Server1.exe localhost:9080
start "Server 2" Debug\Server2.exe localhost:9081
