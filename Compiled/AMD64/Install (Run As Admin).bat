@echo off
color 02
echo Inyecting Driver Hackz
sc create Vanguard binPath= "%~dp0\Vanguard.sys" type= kernel
sc start Vanguard
echo Inyected Hackz
pause
