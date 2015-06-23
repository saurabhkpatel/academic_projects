@echo off
echo.
echo ***********************************************************
echo           Remote Code Management, v1.0 
echo ***********************************************************
echo.

@echo off
start "WPF Application" Debug\CppCli-WPF-App.exe localhost:9080 localhost:9085 localhost:9086
start "Server 1" Debug\Server1.exe localhost:9085
start "Server 2" Debug\Server2.exe localhost:9086
